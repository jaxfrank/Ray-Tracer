#include "TestScene.h"

#include "Camera.h"
#include "Window.h"

#include "TriangleRenderer.h"

TestScene::TestScene(Window* window, std::string name):
Scene(window, name)
{}

TestScene::~TestScene() {}

void TestScene::entry() {
    camera = new Camera(60.0f);

    Triangle* triangle = new Triangle();
    triangle->a = glm::vec3(0.0f, 1.0f, -5.0f);
    triangle->b = glm::vec3(1.0f, 1.0f, -5.0f);
    triangle->c = glm::vec3(1.0f, 0.0f, -5.0f);

    std::array<glm::vec4, 3> colors;
    colors[0] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    colors[1] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    colors[2] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

    renderers.push_back(new TriangleRenderer(triangle, colors));
}

void TestScene::handleEvent(sf::Event e) {

}

void TestScene::update() {

}

void TestScene::exit() {

}