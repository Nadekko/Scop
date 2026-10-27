#include "../include/config.hpp"

Mesh::Mesh(const std::string &path)
{
    loadMesh(path);
    setMesh();
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::draw(unsigned int ID) const
{
    (void) ID;
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

        if (type == "mtllib")
        {
            std::string mtlFile;
            iss >> mtlFile;

            std::string dir;
            size_t slash = path.find_last_of("/\\");
            if (slash != std::string::npos)
                dir = path.substr(0, slash + 1);
            if (!mtlFile.empty())
                loadMaterial(dir + mtlFile);
        }
        else if (type == "usemtl")
        {
            //
        }
        else if (type == "s")
        {
            //
        }
        else if (type == "o")
        {
            //
        }
        else if (type == "v")
        {
            float x, y, z;
            iss >> x >> y >> z;
            positions.push_back(Vec3(x, y, z));
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
    generateUV();
}

void    Mesh::loadMaterial(const std::string &mtlPath)
{
    
    std::ifstream file(mtlPath);

    if (!file.is_open())
    {
        std::cerr << "WARNING::MESH::MTL_NOT_FOUND\n" << mtlPath << std::endl;
        diffuseColor.push_back(Vec3(1.0f, 1.0f, 1.0f)); // default color
        return;
    }

    std::string line;

    while(getline(file, line))
    {
        std::istringstream iss(line);
        std::string type;

        iss >> type;
        if (type == "newmtl")
        {
            // newmtl Material
        }
        else if (type == "Ns")
        {
            // 
        }
        else if (type == "Ka")
        {
            //
        }
        else if (type == "Kd")
        {
            float r, g, b;
            iss >> r >> g >> b;
            diffuseColor.push_back(Vec3(r, g, b));
            if (diffuseColor.empty())
                diffuseColor.push_back(Vec3(1.0f, 1.0f, 1.0f));
        }
        else if (type == "Ks")
        {
            //
        }
        else if (type == "Ni")
        {
            //
        }
        else if (type == "d")
        {
            //
        }
        else if (type == "illum")
        {
            //
        }
    }
}

Vec3 Mesh::getColor() const
{
    if (!diffuseColor.empty())
        return diffuseColor[0];
    return Vec3(1.0f, 1.0f, 1.0f);
}

void    Mesh::generateUV()
{
    float minX = positions[0].x, maxX = positions[0].x;
    float minY = positions[0].y, maxY = positions[0].y;

    for (const Vec3 &p : positions)
    {
        minX = std::min(minX, p.x);
        maxX = std::max(maxX, p.x);
        minY = std::min(minY, p.y);
        maxY = std::max(maxY, p.y);
    }

    float rangeX = maxX - minX;
    float rangeY = maxY - minY;
    for (const Vec3 &p : positions)
    {
        float u = (rangeX == 0.0f) ? 0.5f : ((p.x - minX) / rangeX);
        float v = (rangeY == 0.0f) ? 0.5f : ((p.y - minY) / rangeY);
        posTexture.push_back(Vec2(u, v));
    }
}

void    Mesh::setMesh()
{
    glGenVertexArrays(1, &VAO);
    
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &UVBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    // [x][y][z] | [nx][ny][nz] | [u][v]
    // location 0| location 1   | location 2
    // offset 0  | offset 3     | offset 6
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(Vec3), positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, UVBO);
    glBufferData(GL_ARRAY_BUFFER, posTexture.size() * sizeof(Vec2), posTexture.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(unsigned int), faces.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);

    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(6 * sizeof(float)));
    // glEnableVertexAttribArray(2);
}


