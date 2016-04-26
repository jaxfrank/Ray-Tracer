#include "Mesh.h"


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<int> indices):
vertices(vertices), 
indices(indices)
{}


Mesh::~Mesh() {}
