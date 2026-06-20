#ifndef MAT4_HPP
#define MAT4_HPP

#include <cmath>

class Mat4
{
    public:
        
        //OpenGL attend a tableau plat pour l'envoyer au shader
        float m[16]; // opengl take colomn first (4x4 = 16 valeurs) 

        // accès : m[ligne * 4 + colonne]
        // m[0]  m[1]  m[2]  m[3]
        // m[4]  m[5]  m[6]  m[7]
        // m[8]  m[9]  m[10] m[11]
        // m[12] m[13] m[14] m[15]

        Mat4(float x, float y);
        ~Mat4();
        Mat4(const Mat4 &other);
        
        Mat4& operator=(const Mat4 &other);
        Mat4& operator*(const Mat4 &other);
        Mat4& operator-(const Mat4 &other);
        Mat4& operator+(const Mat4 &other);

        Mat4& operator*=(const Mat4 &other);
        Mat4& operator+=(const Mat4 &other);
        Mat4& operator-=(const Mat4 &other);

        static Mat4 identity();
        static Mat4 rotation_x(float angle);
        static Mat4 roation_y(float angle);
        static Mat4 roation_z(float angle);
        static Mat4 scale(float scaler);
        static Mat4 translation(float x, float y, float z);
        static Mat4 perspective(float fov, float aspect, float near, float far);

        float const& operator()(float x, float y) const;
        float& operator()(float x, float y);


};

#endif