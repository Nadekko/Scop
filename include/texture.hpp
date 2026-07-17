#pragma once

#include "config.hpp"

class Texture
{
    public:
        unsigned int ID;

        Texture(const char *path);
        ~Texture();

        int     parseTexturePath(const char *path);
        void    bind() const;
        void    unbind() const ;
};