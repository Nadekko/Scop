#pragma once

#include <cmath>

class Vec3
{
    public:

        float x;
        float y;
        float z;

        Vec3();
        Vec3(float _x, float _y, float _z);
        ~Vec3();

        Vec3 operator+(const Vec3 &other);
        Vec3 operator-(const Vec3 &other);
        Vec3 operator*(const Vec3 &other);

        float   dot_product(const Vec3& u) const;   // scalaire
        Vec3    cross_product(const Vec3& u) const; // vecteur
        float   norm() const;
};