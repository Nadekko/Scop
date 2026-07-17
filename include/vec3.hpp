#pragma once

#include "config.hpp"

class Vec3
{
    public:

        float x;
        float y;
        float z;

        Vec3();
        Vec3(float _x, float _y, float _z);
        ~Vec3();

        Vec3 operator+(const Vec3 &other) const;
        Vec3 operator-(const Vec3 &other) const;
        Vec3 operator*(const Vec3 &other) const;

        // Vec3 operator*(float scalar) const;
        // Vec3 operator/(float scalar) const;

        float   dot_product(const Vec3& u) const;   // scalaire
        Vec3    cross_product(const Vec3& u) const; // vecteur
        float   norm() const;

        // Vec3 normalized() const;
        // void normalize();
};