#include "Scene.h"


Scene::Scene(Window* window, std::string name):
    window(window),
    name(name)
{}


Scene::~Scene() {}

void Scene::addRenderer(Renderer* renderer) {
    if(renderer == nullptr) return;

    renderers.push_back(renderer);
}
