#ifndef MAT4_HPP
#define MAT4_HPP

#include <cmath>

class Mat4
{
    public:
        float m[16]; // opengl take colomn first

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
        static Mat4 translation(float x, float y, float z);
        static Mat4 perspective(float fov, float aspect, float nera, float far);

        float const& operator()(float x, float y) const;
        float& operator()(float x, float y);


};

#endif