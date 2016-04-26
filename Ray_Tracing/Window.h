#pragma once

#include <unordered_map>
#include <string>

#include <SFML\Graphics.hpp>

#include <glm\glm.hpp>

class Scene;

struct DisplayBuffers {
    int width, height;
    sf::Uint8* colorBuffer;
    float* depthBuffer;
};

class Window {
public:
    Window(int width, int height, std::string title);
    ~Window();

    void run();

    void addScene(Scene* scene);
    void removeScene(std::string name);
    void setCurrentScene(std::string name);

    glm::vec4 getClearColor() const { return clearColor; }
    void setClearColor(glm::vec4 clearColor) { this->clearColor = clearColor; }

    float getClearDepth() const { return clearDepth; }
    void setClearDepth(float clearDepth) { this->clearDepth = clearDepth; }

    void setFrameCap(int fps) { renderWindow->setFramerateLimit(fps); }

private:
    std::unordered_map<std::string, Scene*> scenes;
    Scene* currentScene;

    sf::RenderWindow* renderWindow;
    sf::Texture displayTexture;
    sf::Sprite displaySprite;

    DisplayBuffers* buffers;

    std::string title;
    
    bool running;

    glm::vec4 clearColor;
    float clearDepth;

    void loop();
    void cleanup();

    void clearColorBuffer(glm::u8vec4 color);
    void clearDepthBuffer(float depth);
};

