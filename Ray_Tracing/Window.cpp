#include "Window.h"

#include <exception>

#include "Scene.h"
#include "RayTracer.h"

Window::Window(int width, int height, std::string title):
title(title),
renderWindow(new sf::RenderWindow(sf::VideoMode(width, height), title)),
buffers(new DisplayBuffers),
running(false)
{
    buffers->colorBuffer = new sf::Uint8[width * height * 4];
    buffers->depthBuffer = new float[width * height];
    buffers->width = width;
    buffers->height = height;
}

Window::~Window() {
    delete[] buffers->colorBuffer;
    delete[] buffers->depthBuffer;
    delete buffers;
    delete renderWindow;
    
    auto iter = scenes.begin();
    while(iter != scenes.end()) {
        delete iter->second;
        iter++;
    }
    scenes.clear();
}

void Window::run() {
    if(!displayTexture.create(buffers->width, buffers->height)) {
        throw std::exception("Display Texture failed to create!");
    }
    if(currentScene == nullptr) {
        throw std::exception("Must assign a current state before attempting to run the window!");
    }

    displaySprite.setTexture(displayTexture);
    displaySprite.setScale(sf::Vector2f((float)renderWindow->getSize().x / (float)buffers->width, (float)renderWindow->getSize().y / (float)buffers->height));

    running = true;
    loop();
    cleanup();
}

void Window::loop() {
    //sf::Clock clock;

    while(running) {
        sf::Event e;
        while(renderWindow->pollEvent(e)) {
            if(e.type == sf::Event::Closed) {
                running = false;
            } else {
                currentScene->handleEvent(e);
            }
        }

        currentScene->update();

        //time += clock.getElapsedTime().asSeconds();
        //std::cout << "FPS: " << 1.0f / clock.getElapsedTime().asSeconds() << " Frame Time: " << clock.getElapsedTime().asSeconds() << std::endl;
        //clock.restart();

        //Clear the buffers
        clearColorBuffer(clearColor);
        clearDepthBuffer(clearDepth);
        
        //Perform the race trace
        RayTracer::render(buffers, currentScene);

        //Put pixels from back buffer into "Front Buffer"
        displayTexture.update(buffers->colorBuffer);

        // RENDER THE "FRONT BUFFER" to screen
        //Clear the real window
        renderWindow->clear();
        //Draw the "front buffer" to the window's back buffer
        renderWindow->draw(displaySprite);
        //Swap the real window's buffers
        renderWindow->display();
    }
}

void Window::cleanup() {
    currentScene->exit();
    renderWindow->close();
}

void Window::addScene(Scene* scene) {
    scenes[scene->getName()] = scene;
}

void Window::removeScene(std::string name) {
    if(scenes.find(name) != scenes.end()) {
        scenes.erase(name);
    }
}

void Window::setCurrentScene(std::string name) {
    currentScene->exit();
    currentScene = scenes[name];
    if(currentScene == nullptr) throw new std::exception("Trying to switch to a scene that does not exist!"); // Need to test if this will ever actually happen or if behavior is totally different.
    currentScene->entry();
}

void Window::clearColorBuffer(glm::u8vec4 color) {
    for(int i = 0; i < buffers->width; i++) {
        for(int j = 0; j < buffers->height; j++) {
            int index = (i + j * buffers->width) * (sizeof(glm::u8vec4) / sizeof(sf::Uint8))/*4*/;
            buffers->colorBuffer[index++] = color.r;
            buffers->colorBuffer[index++] = color.g;
            buffers->colorBuffer[index++] = color.b;
            buffers->colorBuffer[index++] = color.a;
        }
    }
}

void Window::clearDepthBuffer(float depth) {
    for(int i = 0; i < buffers->width; i++) {
        for(int j = 0; j < buffers->height; j++) {
            int index = i + j * buffers->width;
            buffers->depthBuffer[index] = depth;
        }
    }
}
