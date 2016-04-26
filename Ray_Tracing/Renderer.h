#pragma once

#include <glm\glm.hpp>

#include "Triangle.h"

class Renderer {
public:
    Renderer() {}
    ~Renderer() {}

    virtual int getTriangleCount() const = 0;
    virtual Triangle& getTriangle(int index) = 0;

    virtual glm::vec4 getColor(int index, float u, float v, float w) const = 0;
};

