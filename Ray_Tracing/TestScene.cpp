#include "TestScene.h"

#include <iostream>

#include <glm\gtc\random.hpp>
#include <glm\glm.hpp>
#include <glm\gtx\rotate_vector.hpp>

#include "Camera.h"
#include "Window.h"
#include "Time.h"

#include "TriangleRenderer.h"

TestScene::TestScene(Window* window, std::string name):
Scene(window, name)
{}

TestScene::~TestScene() {
    delete texture;
}

void TestScene::entry() {
    camera = new Camera(60.0f);
    srand((unsigned int)time(0));

    texture = new sf::Image(); 
    texture->loadFromFile("res/test.png");

    for(int i = 0; i < 20; i++) {
        Triangle* triangle = new Triangle();
        glm::vec3 offset = glm::vec3(glm::diskRand(5.0f), glm::linearRand(-5.0f, 0.0f));
        triangle->a = glm::vec3(0.0f, 1.0f, -5.0f) + glm::linearRand(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f)) + offset;
        triangle->b = glm::vec3(-0.866f, -0.5f, -5.0f) + glm::linearRand(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f)) + offset;
        triangle->c = glm::vec3(0.866f, -0.5f, -5.0f) + glm::linearRand(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f)) + offset;

        std::array<glm::vec4, 3> colors;
        glm::vec3 color = glm::sphericalRand(1.0f);
        glm::abs(color);

        colors[0] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        colors[1] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        colors[2] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        /*
        colors[0] = glm::vec4(color*glm::linearRand(0.5f,1.0f), 1.0f);
        colors[1] = glm::vec4(color*glm::linearRand(0.5f, 1.0f), 1.0f);
        colors[2] = glm::vec4(color*glm::linearRand(0.5f, 1.0f), 1.0f);
        */
        std::array<glm::vec2, 3> texCoords;
        texCoords[0] = glm::vec2(0,0);
        texCoords[1] = glm::vec2(1, 0);
        texCoords[2] = glm::vec2(1, 1);

        renderers.push_back(new TriangleRenderer(triangle, colors, texture, texCoords));
    }
}

void TestScene::handleEvent(sf::Event e) {
    switch(e.type) {
    case sf::Event::KeyPressed:
        switch(e.key.code) {
        case sf::Keyboard::W:
            camera->setPosition(camera->getPosition() + camera->getForward() * cameraMovementSpeed * Time::getDelta());
            break;
        case sf::Keyboard::A:
            camera->setPosition(camera->getPosition() - glm::normalize(glm::cross(camera->getForward(), glm::vec3(0.0f, 1.0f, 0.0f))) * cameraMovementSpeed * Time::getDelta());
            break;
        case sf::Keyboard::S:
            camera->setPosition(camera->getPosition() - camera->getForward() * cameraMovementSpeed * Time::getDelta());
            break;
        case sf::Keyboard::D:
            camera->setPosition(camera->getPosition() + glm::normalize(glm::cross(camera->getForward(), glm::vec3(0.0f, 1.0f, 0.0f))) * cameraMovementSpeed * Time::getDelta());
            break;
        case sf::Keyboard::Escape:
            window->setMouseCaptured(false);
            break;
        }
        break;
    case sf::Event::MouseButtonPressed:
        switch(e.mouseButton.button) {
        case sf::Mouse::Left:
            window->setMouseCaptured(true);
            break;
        }
        break;
    }
}

void TestScene::update() {
    if(window->isMouseCaptured()) {
        float x = (float)(sf::Mouse::getPosition().x - window->getPosition().x) / (float)window->getWidth() * 2.0f - 1.0f;
        float y = (float)(sf::Mouse::getPosition().y - window->getPosition().y) / (float)window->getHeight() * 2.0f - 1.0f;
        camera->setForward(glm::normalize(glm::rotate(camera->getForward(), -glm::sin(x) * Time::getDelta() * 10.0f, glm::vec3(0.0f, 1.0f, 0.0f))));
        camera->setForward(glm::normalize(glm::rotate(camera->getForward(), -glm::sin(y) * Time::getDelta() * 10.0f, camera->getRight())));
    }
}

void TestScene::exit() {

}