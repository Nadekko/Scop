#pragma once

#include "config.hpp"

class Texture
{
    public:
        unsigned int ID;

        Texture(const std::string &path);
        ~Texture();

        // int     parseTexturePath(const char *path);
        void    bind() const;
        void    unbind() const ;
};