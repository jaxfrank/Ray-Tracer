#pragma once

#include "Scene.h"

#include <string>

class Window;
class Camera;

class TestScene : public Scene {
public:
    TestScene(Window* window, std::string name);
    ~TestScene();

    virtual void entry();
    virtual void handleEvent(sf::Event e);
    virtual void update();
    virtual void exit();

private:
    sf::Image* texture;
};

