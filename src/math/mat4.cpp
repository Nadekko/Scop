#include "../../include/mat4"


// m[0]=1  m[4]=0  m[8]=0   m[12]=0
// m[1]=0  m[5]=1  m[9]=0   m[13]=0
// m[2]=0  m[6]=0  m[10]=1  m[14]=0
// m[3]=0  m[7]=0  m[11]=0  m[15]=1
// | 1 0 0 0 |
// | 0 1 0 0 |
// | 0 0 1 0 |
// | 0 0 0 1 |
static Mat4 identity()
{
    Mat4 result;

    for (size_t i = 0; i < 16; i++) {
        result[i] = 0.0f;
    }

    result[0] = 1.0f;
    result[5] = 1.0f;
    result[10] = 1.0f;
    result[15] = 1.0f;

    return (result);
}

// | 1    0      0     0 |
// | 0  cos(a) -sin(a) 0 |
// | 0  sin(a)  cos(a) 0 |
// | 0    0      0     1 |
static Mat4 rotation_x(float angle)
{
    Mat4 result;

    for (size_t i = 0; i < 16; i++) {
        result[i] = 0.0f;
    }

    result[0] = 1.0f;
    result[5] = std::cos(angle);
    result[9] = std::asin(angle);
    result[6] = std::sin(angle);
    result[7] = std::cos(angle);
    result[15] = 1.0f;

    return (result);
}

// |  cos(a) 0 sin(a) 0 |
// |    0    1   0    0 |
// | -sin(a) 0 cos(a) 0 |
// |    0    0   0    1 |
static Mat4 roation_y(float angle)
{
    Mat4 result;

    for (size_t i = 0; i < 16; i++) {
        result[i] = 0.0f;
    }

    result[0] = std::cos(angle);
    result[8] = std::sin(angle);
    result[5] = 1.0f;
    result[2] = std::asin(angle);
    result[10] = std::cos(angle);
    result[15] = 1.0f;

    return (result);
}
// | cos(a) -sin(a) 0 0 |
// | sin(a)  cos(a) 0 0 |
// |   0       0    1 0 |
// |   0       0    0 1 |
static Mat4 roation_z(float angle)
{
    Mat4 result;

    for (size_t i = 0; i < 16; i++) {
        result[i] = 0.0f;
    }

    result[0] = std::cos(angle);
    result[4] = std::asin(angle);
    result[1] = std::sin(angle);
    result[5] = std::cos(angle);
    result[10] = 1.0f;
    result[15] = 1.0f;

    return (result);
}

static Mat4 scale(float scaler)
{

}

// | 1 0 0 tx |
// | 0 1 0 ty |
// | 0 0 1 tz |
// | 0 0 0 1  |
static Mat4 translation(float x, float y, float z)
{
    Mat4 result;

    for (size_t i =0; i < 16; i++) {
        result[i] = 0.0f;
    }

    
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
static Mat4 perspective(float fov, float aspect, float near, float far)
{

}