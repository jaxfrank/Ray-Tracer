#include "TriangleRenderer.h"

#include <glm\glm.hpp>

TriangleRenderer::TriangleRenderer(Triangle* triangle, std::array<glm::vec4, 3> colors, sf::Image* texture, std::array<glm::vec2, 3> texCoords):
triangle(triangle),
colors(colors),
texture(texture),
texCoords(texCoords)
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
    glm::vec2 texCoord((u*texCoords[0] + v*texCoords[1] + w*texCoords[2]) * glm::vec2(texture->getSize().x, texture->getSize().y));
    sf::Color color = texture->getPixel(texCoord.x, texCoord.y);
    return (u*colors[0] + v*colors[1] + w*colors[2]) * glm::vec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
}