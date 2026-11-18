#pragma once

#include "config.hpp"

class Mesh
{
    public:

        Mesh(const std::string &objPath);
        ~Mesh();

        void    draw() const;
        Vec3    getCenter() const;
        float   getScale() const;               

    private:
        // CPU
        std::vector<Vec3>           positions; // v
        std::vector<Vec3>           diffuseColor; // Kd
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
        void    setMesh();
        void    computeBound();
        void    buildVertices();
};