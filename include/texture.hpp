#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>

#include <string>
#include <iostream>
#include <cstdio>

class Texture {

    public:
        unsigned int ID;

        Texture(const char* path);
        ~Texture();

        void bind() const;
        void unbind() const;
};

#endif