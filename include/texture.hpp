#pragma once

#include "config.hpp"

class Texture
{
    public:
        unsigned int ID;

        Texture(const char *path);
        ~Texture();

        void bind() const;
        void unbind() const ;
};