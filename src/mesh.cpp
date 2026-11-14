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
}

void Mesh::draw(unsigned int ID) const
{
    (void) ID;
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()) / 8);
    glBindVertexArray(0);
}

// Converts one face token to a 0-based index into positions.
// Accepted forms: "7", "7/2", "7//5", "7/2/5" -> only the vertex index (before the first '/') is used.
// OBJ indices start at 1; a negative index counts backwards from the last vertex read so far (-1 = last).
static unsigned int parseFaceIndex(const std::string &token, size_t vertexCount, size_t lineNum)
{
    std::string error = "ERROR::MESH::INVALID_FACE_INDEX '" + token + "' line " + std::to_string(lineNum);

    // keep only what is before the first '/' ("7/2/5" -> "7"); without '/', find returns npos and substr keeps everything
    std::istringstream iss(token.substr(0, token.find('/')));
    long idx;

    if (!(iss >> idx) || !iss.eof())     // not a number, or garbage after it ("12abc")
        throw std::runtime_error(error);

    if (idx == 0)                        // 0 does not exist in OBJ
        throw std::runtime_error(error);
    if (idx > 0)
        idx = idx - 1;                   // 1-based -> 0-based
    else
        idx = static_cast<long>(vertexCount) + idx;  // -1 -> last vertex

    if (idx < 0 || idx >= static_cast<long>(vertexCount))
        throw std::runtime_error(error);
    return static_cast<unsigned int>(idx);
}

void Mesh::loadMesh(const std::string &path)
{
    if (path.length() < 4 || path.substr(path.length() - 4) != ".obj")
        throw std::runtime_error("ERROR::MESH::UNSUPPORTED_FILE_FORMAT");
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("ERROR::MESH::FILE_NOT_SUCCESFULLY_READ");
    
    std::string line;
    size_t lineNum = 0;
    unsigned int faceCount = 0;
    while (std::getline(file, line))
    {
        lineNum++;
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
        else if (type == "v") // vertex : position (x, y, z)
        {
            float x, y, z;
            if (!(iss >> x >> y >> z))
                throw std::runtime_error("ERROR::MESH::INVALID_VERTEX line " + std::to_string(lineNum));
            positions.push_back(Vec3(x, y, z));
        }
        else if (type == "f") // faces
        {
            std::vector<unsigned int> indices;
            std::string token;
            
            while (iss >> token)
                indices.push_back(parseFaceIndex(token, positions.size(), lineNum));
            if (indices.size() < 3)
                throw std::runtime_error("ERROR::MESH::FACE_NEEDS_3_VERTICES line " + std::to_string(lineNum));

            for (size_t f = 1;f + 1 < indices.size(); f++)
            {
                faces.push_back(indices[0]);
                faces.push_back(indices[f]);
                faces.push_back(indices[f + 1]);
                triangleFace.push_back(faceCount);
            }
            faceCount++;
        }
        else if (type == "vn") // vertex texture
        {
            // TODO (bonus)
        }
        else if (type == "vt") // vertex normal
        {
            // TODO (bonus
        }
    }
    if (faces.empty())
        throw std::runtime_error("ERROR::MESH::NO_FACES: " + path);
    computeBound();
    buildVertices();
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

Vec3 Mesh::getCenter() const
{
    return center;
}

float Mesh::getScale() const
{
    return scale;
}

// Computes the axis-aligned bounding box of the mesh, once at load time:
// - center: middle of the box, used to rotate the object around itself
// - scale: factor that makes the largest side of the box 2 units long,
//   so every model fits in the camera whatever its original size
void Mesh::computeBound()
{
    if (positions.empty())
        throw std::runtime_error("ERROR::MESH::NO_VERTICES");

    float minX = positions[0].x, maxX = positions[0].x;
    float minY = positions[0].y, maxY = positions[0].y;
    float minZ = positions[0].z, maxZ = positions[0].z;

    for (const Vec3 &p : positions)
    {
        minX = std::min(minX, p.x);
        maxX = std::max(maxX, p.x);
        minY = std::min(minY, p.y);
        maxY = std::max(maxY, p.y);
        minZ = std::min(minZ, p.z);
        maxZ = std::max(maxZ, p.z);
    }

    float x = (minX + maxX) / 2.0f;
    float y = (minY + maxY) / 2.0f;
    float z = (minZ + maxZ) / 2.0f;

    center = Vec3(x, y, z);

    // size of the box on each axis
    float sizeX = maxX - minX;
    float sizeY = maxY - minY;
    float sizeZ = maxZ - minZ;
    
    float maxExtent = std::max(sizeX, std::max(sizeY, sizeZ));

    scale = (maxExtent == 0) ? 1.0f : 2.0f / maxExtent;

    boundMin = Vec3(minX, minY, minZ);
    boundSize = Vec3(sizeX, sizeY, sizeZ);
}

void    Mesh::buildVertices()
{
    for (size_t t = 0; t < triangleFace.size(); t++)
    {
        float gray = 0.2f + 0.15f * (triangleFace[t] % 5); // 5 gray level
        for (size_t k = 0; k < 3; k++)
        {
            unsigned int idx = faces[t * 3 + k];
            if (idx >= positions.size())
                throw (std::runtime_error("ERROR::MESH_INVALID_FACE_INDEX"));
            const Vec3 &p = positions[idx];

            // u : horizontal axis = the widest one between x and z
            float u;
            if (boundSize.z > boundSize.x)
                u = (p.z - boundMin.z) / boundSize.z;
            else
                u = (boundSize.x == 0.0f) ? 0.5f : (p.x - boundMin.x) / boundSize.x;
            // v : vertical axis = y
            float v = (boundSize.y == 0.0f) ? 0.5f : (p.y - boundMin.y) / boundSize.y;

            vertices.push_back(p.x);
            vertices.push_back(p.y);
            vertices.push_back(p.z);
            vertices.push_back(gray);
            vertices.push_back(gray);
            vertices.push_back(gray);
            vertices.push_back(u);
            vertices.push_back(v);
        }
    }
}

void    Mesh::setMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO); // VAO records the layout below
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // VBO becomes the current buffer
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    // [x][y][z] | [nx][ny][nz] | [u][v]
    // location 0| location 1   | location 2
    // offset 0  | offset 3     | offset 6
    // tell the GPU how to cut the array
    GLsizei stride = 8 * sizeof(float);

    // location 0 : position (3 float, offset 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    // location 1 : color (3 float, offset 3)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // location 2 : uv (2 float, offset 6)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}


