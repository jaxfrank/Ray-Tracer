#include <iostream>

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <SFML\Graphics.hpp>

#include <vector>

#include "Vertex.h"

int main(int argc, char* argv[]) {

    /*
    Camera camera;
    camera.position = glm::vec3(0.0f, 0.0f, 0.0f);
    camera.forward = glm::vec3(0.0f, 0.0f, -1.0f);
    camera.fov = 60.0f;
    */
    /*
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
    */
       
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
    return 0;
}