#pragma once

#include <Vector>

#include "Vertex.h"

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<int> indices);
    ~Mesh();

private:
    std::vector<Vertex> vertices;
    std::vector<int> indices;

};
