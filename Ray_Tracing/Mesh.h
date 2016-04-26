#ifndef MESH_H
#define MESH_H

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

#endif