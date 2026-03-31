#include "../include/mesh.hpp"

Mesh::Mesh(const std::string &path)
{
    loadMesh(path);
    setupMesh();
}

Mesh::~Mesh()
{
}

void Mesh::loadMesh(const std::string &path)
{
    if (path.length() < 4 || path.substr(path.length() - 4) != ".obj")
    {
        std::cerr << "ERROR::MESH::UNSUPPORTED_FILE_FORMAT\n" << path << std::endl;
        return;
    }

    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "ERROR::MESH::FILE_NOT_SUCCESFULLY_READ\n" << path << std::endl;
        return;
    }
    
    std::string line;
    while (getline(file, line))
    {
        if (line[0] == 'v')
        {
            // Process vertex data
        }
        else if (line[0] == 'f')
        {
            // Process face data
        }
    }
}

