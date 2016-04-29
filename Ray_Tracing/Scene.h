#pragma once

#include <SFML\Graphics.hpp>

#include <string>
#include <vector>

class Window;
class Camera;
class Renderer;

class Scene {
public:
    Scene();
    Scene(Window* window, std::string name);
    ~Scene();

    virtual void entry() = 0;
    virtual void handleEvent(sf::Event e) = 0;
    virtual void update() = 0;
    virtual void exit() = 0;

    std::string getName() const { return name; }
    
    Camera* getCamera() { return camera; }
    void setCamera(Camera* camera) { this->camera = camera; }

    void addRenderer(Renderer* renderer);
    std::vector<Renderer*>& getRenderers() { return renderers; }

protected:
    Window* window;
    std::string name;
    Camera* camera;
    std::vector<Renderer*> renderers;

};

