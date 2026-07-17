#pragma once

#include "config.hpp"

class Mat4
{
    public:
        
        //OpenGL attend a tableau plat pour l'envoyer au shader
        float m[16]; // opengl par colonne (4x4 = 16 valeurs) 

        Mat4();
        ~Mat4();
        Mat4(const Mat4 &other);
        
        Mat4 &operator=(const Mat4 &other);
        Mat4 &operator*=(const Mat4 &other);
        Mat4 operator*(const Mat4 &other);

        static Mat4 identity();
        static Mat4 rotation_x(float angle);
        static Mat4 rotation_y(float angle);
        static Mat4 rotation_z(float angle);
        static Mat4 scale(float scaler);
        static Mat4 translation(float x, float y, float z);
        static Mat4 perspective(float fov, float aspect, float near, float far);

        float const& operator()(float x, float y) const;
        float& operator()(float x, float y);


};