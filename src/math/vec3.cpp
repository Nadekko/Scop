#include "../../include/config.hpp"

Vec3::Vec3() {}

Vec3::Vec3(float _x, float _y, float _z)
{
    x = _x;
    y = _y;
    z = _z;
}

Vec3::~Vec3() {}

Vec3 Vec3::operator+(const Vec3 &other)
{
    return (Vec3(x + other.x, y + other.y, z + other.z));
}

Vec3 Vec3::operator-(const Vec3 &other)
{
    return (Vec3(x - other.x, y - other.y, z - other.z));
}

Vec3 Vec3::operator*(const Vec3 &other)
{
    return (Vec3(x * other.x, y * other.y, z * other.z));
}

float Vec3::dot_product(const Vec3 &u) const
{
    return (x * u.x + y * u.y + z * u.z);
}

Vec3 Vec3::cross_product(const Vec3& u) const
{
    return (Vec3(
        y * u.z - z * u.y,
        z * u.x - x * u.z,
        x * u.y - y * u.x
    ));
}

float Vec3::norm() const
{
    return std::sqrt(x*x + y*y + z*z);
}