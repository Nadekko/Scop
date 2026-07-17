#pragma once

#include "config.hpp"

// struct Vertex
// {
//     float position[3]; // v
//     float texCoords[2]; // vt :: UV
//     float normale[3]; // vn
// };

class Mesh
{
    public:

        Mesh(const std::string &objPath);
        ~Mesh();

        void draw() const; 

    private:
        // CPU
        std::vector<float> vertices; // v
        std::vector<unsigned int> faces; // f

        // GPU
        unsigned int VAO, VBO, EBO;
        
        void loadMesh(const std::string &path);
        void setupMesh();
};