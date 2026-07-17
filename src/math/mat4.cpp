#include "../../include/config.hpp"

Mat4::Mat4()
{
    for (size_t i = 0; i < 16; i++) {
        m[i] = 0.0f;
    }
}

Mat4::Mat4(const Mat4 &other)
{
    for (size_t i = 0; i < 16; i++) {
        m[i] = other.m[i];
    }
}

Mat4 Mat4::operator*(const Mat4 &other) const
{
    Mat4 mut;
    
    for (size_t col = 0; col < 4; col++) {
        for (size_t row = 0; row < 4; row++) {
            for (size_t k = 0; k < 4; k++) {
                mut.m[col * 4 + row] = fmaf(m[k * 4 + row], other.m[col * 4 + k], mut.m[col * 4 + row]);
            }
        }
    }
    return (mut);
}

Mat4 &Mat4::operator*=(const Mat4 &other)
{
    *this = *this * other;
    return (*this);
}

Mat4 &Mat4::operator=(const Mat4 &other)
{
    for (size_t i = 0; i < 16; i++) {
        m[i] = other.m[i];
    }
    return (*this);
}

Mat4::~Mat4() {}
// m[0]=1  m[4]=0  m[8]=0   m[12]=0
// m[1]=0  m[5]=1  m[9]=0   m[13]=0
// m[2]=0  m[6]=0  m[10]=1  m[14]=0
// m[3]=0  m[7]=0  m[11]=0  m[15]=1
// | 1 0 0 0 |
// | 0 1 0 0 |
// | 0 0 1 0 |
// | 0 0 0 1 |
Mat4 Mat4::identity()
{
    Mat4 I;

    I.m[0] = 1.0f;
    I.m[5] = 1.0f;
    I.m[10] = 1.0f;
    I.m[15] = 1.0f;

    return (I);
}
// Mathématique (row-major) :     En mémoire column-major (m[col*4+row]) :
// |  c  0  s  0 |                col0    col1    col2    col3
// |  0  1  0  0 |       →     [ c,0,-s,0 | 0,1,0,0 | s,0,c,0 | 0,0,0,1 ]
// | -s  0  c  0 |
// |  0  0  0  1 |
// | 1    0      0     0 |
// | 0  cos(a) -sin(a) 0 |
// | 0  sin(a)  cos(a) 0 |
// | 0    0      0     1 |
Mat4 Mat4::rotation_x(float angle)
{
    Mat4 R_x = Mat4::identity();
    float c = std::cos(angle);
    float s = std::sin(angle);

    R_x.m[5] = c;
    R_x.m[6] = s;
    R_x.m[9] = -s;
    R_x.m[10] = c;

    return (R_x);
}

// |  cos(a) 0 sin(a) 0 |
// |    0    1   0    0 |
// | -sin(a) 0 cos(a) 0 |
// |    0    0   0    1 |
Mat4 Mat4::rotation_y(float angle)
{
    Mat4 R_y = Mat4::identity();
    float c = std::cos(angle);
    float s = std::sin(angle);

    R_y.m[0] = c;
    R_y.m[2] = -s;
    R_y.m[8] = s;
    R_y.m[10] = c;

    return (R_y);
}
// | cos(a) -sin(a) 0 0 |
// | sin(a)  cos(a) 0 0 |
// |   0       0    1 0 |
// |   0       0    0 1 |
Mat4 Mat4::rotation_z(float angle)
{
    Mat4 R_z = Mat4::identity();
    float c = std::cos(angle);
    float s = std::sin(angle);

    R_z.m[0] = c;
    R_z.m[1] = s;
    R_z.m[4] = -s;
    R_z.m[5] = c;

    return (R_z);
}

Mat4 Mat4::scale(float scaler)
{
    Mat4 S = Mat4::identity();

    S.m[0] = scaler;
    S.m[5] = scaler;
    S.m[10] = scaler;

    return (S);
}
// | 1 0 0 tx |
// | 0 1 0 ty |
// | 0 0 1 tz |
// | 0 0 0 1  |
Mat4 Mat4::translation(float x, float y, float z)
{
    Mat4 T = Mat4::identity();

    T.m[12] = x;
    T.m[13] = y;
    T.m[14] = z;

    return (T);
}

// ratio : ratio de la fenêtre (la largeur de la fenêtre divisé par sa hauteur)
// angle : angle de vue de la caméra (70° pour un oeil humain)
// near : distance la plus proche de la caméra qu'il faut afficher
// far : distance la plus éloignée de la caméra qu'il faut afficher
// f = 1.0 / tan(fov / 2.0)
// P = [
//   f/ratio,  0,   0,              0,
//   0,        f,   0,              0,
//   0,        0,   far/(near-far), -1,
//   0,        0,   (near*far)/(near-far), 0
// ]
Mat4 Mat4::perspective(float fov, float aspect, float near, float far)
{

    Mat4 P;

    float f = 1 / std::tan(fov / 2);

    P.m[0] = f / aspect;
    P.m[5] = f;
    P.m[10] = (far + near) / (near - far);
    P.m[11] = -1.0f;
    P.m[14] = (2.0f * near * far) / (near - far);

    return (P);
}
