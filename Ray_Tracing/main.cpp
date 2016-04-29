#include <iostream>

#include "Window.h"
#include "TestScene.h"

int main(int argc, char* argv[]) {
       
    /* Move something around
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
    */
    
    Window* window = new Window(1280, 720, "The Ray Caster!");
    window->addScene(new TestScene(window, "TestScene"));
    window->setFrameCap(0);
    window->setCurrentScene("TestScene");

    window->run();

    return 0;
}