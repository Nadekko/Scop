#include "../include/config.hpp"

// Reads a whole text into a string
static std::string readFile(const std::string &path)
{
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("ERROR::SHADER::CANNOT_OPEN_FILE: " + path);
    std::stringstream buffer;
    buffer << file.rdbuf(); // copy the whole file
    return buffer.str();
}

// Compile one shader stage (vertex or fragment)
// On failure: frees the shader, then throws with the GLSL compiler message 
static unsigned int compileShader(GLenum type, const std::string &source, const std::string & path)
{
    unsigned int shader = glCreateShader(type);
    const char *code = source.c_str();

    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        glDeleteShader(shader); // created on the GPU: free it before throwing
        throw std::runtime_error("ERROR::SHADER::COMPILATION_FAILED: " + path + "\n" + infoLog);
    }
    return shader;
}

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
{
    // 1. read both file (nothing exist on the GPU yet)
    std::string vertexCode = readFile(vertexPath);
    std::string fragmentCode = readFile(fragmentPath);

    // 2. compile both stage
    unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexCode, vertexPath);
    unsigned int fragment = 0;
    try
    {
        fragment = compileShader(GL_FRAGMENT_SHADER, fragmentCode, fragmentPath);
    }
    catch (...) // execption
    {
        glDeleteShader(vertex); // vertex already exist on GPU
        throw; // intercepted by main
    }
    // 3. link both stage into one program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // delete the shaders as they're linked into our program now and no longer necessary
    // the program keeps what it needs
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    int success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetProgramInfoLog(ID, 1024, NULL, infoLog);
        glDeleteProgram(ID);
        throw std::runtime_error(std::string("ERROR::SHADER::LINKING_FAILED\n") + infoLog);
    }
}

Shader::~Shader()
{
    glDeleteProgram(ID);
}

void Shader::use() const
{
    glUseProgram(ID);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
