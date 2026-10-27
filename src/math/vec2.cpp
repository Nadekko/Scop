#include "../../include/config.hpp"

Vec2::Vec2() : x(0.0f), y(0.0f) {}

Vec2::Vec2(float _x, float _y)
{
    x = _x;
    y = _y;
}

Vec2::~Vec2() {}

Vec2 Vec2::operator+(const Vec2 &other) const
{
    return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(const Vec2 &other) const
{
    return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator*(const Vec2 &other) const
{
    return Vec2(x * other.x, y * other.y);
}

