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

        void draw(unsigned int shaderID) const;
        Vec3                 getColor() const;

    private:
        // CPU
        std::vector<Vec3>   positions; // v
        std::vector<Vec3>   normales; // vn
        std::vector<Vec3>   diffuseColor; // Kd
        std::vector<Vec2>   posTexture; // uv
        std::vector<unsigned int> faces; // f

        // GPU
        unsigned int VAO, VBO, EBO, UVBO;
        
        void    loadMesh(const std::string &path);
        void    loadMaterial(const std::string &mtlPath);
        void    generateUV();
        // void    loadLighting(const std::string &mtlPath);
        void    setMesh();
};