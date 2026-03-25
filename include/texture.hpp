#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <iostream>
#include <glad/glad.h>

class Texture
{
    public:
        unsigned int ID;

        Texture(const char *path);
        ~Texture();

        void bind() const;
        void unbind() const ;
};

#endif