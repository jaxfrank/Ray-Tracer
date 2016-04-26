#include <iostream>

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <SFML\Graphics.hpp>

#include <vector>

#include "Vertex.h"

static const int width = 854;
static const int height = 480;
static const int numCores = 6;

static glm::u8vec4 clearColor(0, 255, 255, 255);

int main(int argc, char* argv[]) {
    sf::RenderWindow window(sf::VideoMode(width, height), "Ray Tracing!");
    window.setFramerateLimit(60);

    sf::Texture texture;
    if(!texture.create(width, height)) {
        return -1;
    }
    sf::Uint8* pixels = new sf::Uint8[width * height * 4];

    clear(pixels, clearColor);
    
    texture.update(pixels);

    sf::Sprite sprite(texture);
    sprite.setScale(sf::Vector2f((float)window.getSize().x / (float)width, (float)window.getSize().y / (float)height));

    Camera camera;
    camera.aspectRatio = (float)width / (float)height;
    camera.position = glm::vec3(0.0f, 0.0f, 0.0f);
    camera.forward = glm::vec3(0.0f, 0.0f, -1.0f);
    camera.fov = 60.0f;

    Triangle triangle;
    Vertex a;
    a.position = glm::vec3(0.0f, 1.0f, -5.0f);
    a.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    triangle.a = a;
    Vertex b;
    b.position = glm::vec3(1.0f, 1.0f, -5.0f);
    b.color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    triangle.b = b;
    Vertex c;
    c.position = glm::vec3(1.0f, 0.0f, -5.0f);
    c.color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    triangle.c = c;
    
    float time = 0;

    sf::Clock clock;
    while(window.isOpen()) {
        sf::Event e;
        while(window.pollEvent(e)) {
            if(e.type == sf::Event::Closed) {
                window.close();
            }
        }

        time += clock.getElapsedTime().asSeconds();
        std::cout << "FPS: " << 1.0f / clock.getElapsedTime().asSeconds() << " Frame Time: " << clock.getElapsedTime().asSeconds() << std::endl;
        clock.restart();
        
        float motionX = 2.0f*glm::sin(time);
        float motionZ = -2.0f*glm::cos(time);
        
        glm::mat4 transform = glm::translate(glm::vec3(motionX, 0.0f, motionZ));
        transform = glm::rotate(transform, time, glm::vec3(0.0f, 0.0f, 1.0f));

        Triangle temp = triangle;
        glm::vec4 translated = transform * glm::vec4(temp.a.position, 1.0f);
        temp.a.position = glm::vec3(translated.x, translated.y, translated.z);

        translated = transform * glm::vec4(temp.b.position, 1.0f);
        temp.b.position = glm::vec3(translated.x, translated.y, translated.z);

        translated = transform * glm::vec4(temp.c.position, 1.0f);
        temp.c.position = glm::vec3(translated.x, translated.y, translated.z);
        
        //Set all pixels to clearColor in "Back Buffer"
        clear(pixels, clearColor);
        //Perform the race trace
        trace(pixels, camera, temp);
        //Put pixels from back buffer into "Front Buffer"
        texture.update(pixels);

        // RENDER THE "FRONT BUFFER" to screen
        //Clear the real window
        window.clear();
        //Draw the "front buffer" to the window's back buffer
        window.draw(sprite);
        //Swap the real window's buffers
        window.display();
    }

    return 0;
}