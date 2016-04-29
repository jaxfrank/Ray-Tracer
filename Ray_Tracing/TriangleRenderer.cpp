#include "TriangleRenderer.h"

#include <glm\glm.hpp>

TriangleRenderer::TriangleRenderer(Triangle* triangle, std::array<glm::vec4, 3> colors):
triangle(triangle),
colors(colors)
{}


TriangleRenderer::~TriangleRenderer() {
    delete triangle;
}

int TriangleRenderer::getTriangleCount() const {
    return 1;
}

Triangle* TriangleRenderer::getTriangle(int index) {
    return triangle;
}

glm::vec4 TriangleRenderer::getColor(int index, float u, float v, float w) const {
    return u*colors[0] + v*colors[1] + w*colors[2];
}