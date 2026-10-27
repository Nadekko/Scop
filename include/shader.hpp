#pragma once

#include "config.hpp"

class Shader
{
    public :
        // program ID
        unsigned int ID;
        // constructor reads and builds the shader
        Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
        ~Shader();
        // use/activate the shader
        void    use() const;
        // utility uniform functions
        void    setBool(const std::string &name, bool value) const;
        void    setInt(const std::string &name, int value) const;
        void    setFloat(const std::string &name, float value) const;
        void    setVec3(const std::string &name, const Vec3 &value) const;

    private:
        void    checkCompileErrors(unsigned int shader, std::string type);
};