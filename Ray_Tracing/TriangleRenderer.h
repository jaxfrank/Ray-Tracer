#pragma once

#include "Renderer.h"

#include <array>
#include <glm\glm.hpp>

struct Triangle;

class TriangleRenderer : public Renderer {
public:
    TriangleRenderer(Triangle* triangle, std::array<glm::vec4, 3> colors);
    ~TriangleRenderer();


    inline virtual int getTriangleCount() const;
    virtual Triangle* getTriangle(int index);

    virtual glm::vec4 getColor(int index, float u, float v, float w) const;

private:
    Triangle* triangle;
    std::array<glm::vec4, 3> colors;

};

