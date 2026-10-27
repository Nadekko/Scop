#pragma once

#include "config.hpp"

class Vec2
{
    public:
        float x, y;

        Vec2();
        Vec2(float x, float y);
        ~Vec2();

        Vec2    operator+(const Vec2 &other) const;
        Vec2    operator-(const Vec2 &other) const;
        Vec2    operator*(const Vec2 &other) const;
};