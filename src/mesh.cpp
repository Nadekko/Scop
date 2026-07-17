#include "../include/config.hpp"

Mesh::Mesh(const std::string &path)
{
    loadMesh(path);
    setupMesh();
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::draw() const
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(faces.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
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
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v")
        {
            float x, y, z;
            iss >> x >> y >> z;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
        else if (type == "f")
        {
            std::vector<unsigned int> indices;
            unsigned int idx;

            while (iss >> idx)
                indices.push_back(idx - 1);
            for (size_t i = 1; i + 1 < indices.size(); i++)
            {
                faces.push_back(indices[0]);
                faces.push_back(indices[i]);
                faces.push_back(indices[i + 1]);
            }
        }
    }
}


void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(unsigned int), faces.data(), GL_STATIC_DRAW);


    // [x][y][z] | [nx][ny][nz] | [u][v]
    // location 0| location 1   | location 2
    // offset 0  | offset 3     | offset 6

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);

    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);

    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(6 * sizeof(float)));
    // glEnableVertexAttribArray(2);
}


