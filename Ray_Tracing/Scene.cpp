#include "Scene.h"

#include "Renderer.h"

Scene::Scene() {}

Scene::Scene(Window* window, std::string name):
window(window),
name(name)
{}

Scene::~Scene() {
    while(renderers.size() > 0) {
        if(renderers.back() != nullptr) {
            delete renderers.back();
        }
        renderers.pop_back();
    }
}

void Scene::addRenderer(Renderer* renderer) {
    if(renderer == nullptr) return;

    renderers.push_back(renderer);
}
