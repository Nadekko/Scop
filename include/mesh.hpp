#ifndef MESH_HPP
#define MESH_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

struct Vertex
{
    float position[3]; // v maybe [4] ?
    float texCoords[2]; // vt
    float normale[3]; // vn
};

class Mesh
{
    public:

        Mesh(const std::string &path);
        ~Mesh();

        void draw() const; 

    private:
        // CPU
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices; // f

        // GPU
        unsigned int VAO, VBO, EBO;
        
        void loadMesh(const std::string &path);
        void setupMesh();
};

#endif // MESH_HPP