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

        void    draw(unsigned int shaderID) const;
        Vec3    getColor() const;
        Vec3    getCenter() const;
        float   getScale() const;               

    private:
        // CPU
        std::vector<Vec3>           positions; // v
        std::vector<Vec3>           normales; // vn
        std::vector<Vec3>           diffuseColor; // Kd
        // std::vector<Vec2>           posTexture; // uv
        std::vector<float>          vertices; // final GPU data: [x y z | r g b | u v] per vertex
        std::vector<unsigned int>   faces; // 3 indices into position per triangle
        std::vector<unsigned int>   triangleFace; // face number of each triangle
        Vec3                        boundMin; // lowest corner of bounding box
        Vec3                        boundSize; // size of bounding box on each axes
        Vec3                        center;
        float                       scale;

        // GPU
        unsigned int VAO, VBO;
        
        void    loadMesh(const std::string &path);
        void    loadMaterial(const std::string &mtlPath);
        void    generateUV();
        // void    loadLighting(const std::string &mtlPath);
        void    setMesh();
        void    computeBound();
        void    buildVertices();
};