#pragma once

#include <cmath>

struct Vec3
{
    union
    {
        double e[3];
        struct
        {
            double x, y, z;
        };
        struct
        {
            double r, g, b;
        };
    };

    Vec3()
        : e{0, 0, 0} {}
    Vec3(double x, double y, double z)
        : e{x, y, z} {}

    Vec3 operator-() const { return Vec3({-x, -y, -z}); }
    double operator[](int i) const { return e[i]; }
    double &operator[](int i) { return e[i]; }

    // utility functions
    Vec3 operator+(const Vec3 &other) const
    {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }
    Vec3 operator-(const Vec3 &other) const
    {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }
    // very very weird
    Vec3 operator*(const Vec3 &other) const
    {
        return Vec3(x * other.x, y * other.y, z * other.z);
    }
    Vec3 operator*(const double scalar) const
    {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }
    Vec3 operator/(const double scalar) const
    {
        return *this * (1 / scalar);
    }
    Vec3 unit_vector() const
    {
        return *this / magnitude();
    }

    Vec3 &operator+=(const Vec3 &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    Vec3 &operator-=(const Vec3 &other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    Vec3 &operator*=(const double scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    Vec3 &operator/=(const double num)
    {
        x *= 1 / num;
        y *= 1 / num;
        z *= 1 / num;
        return *this;
    }

    double magnitute_squared() const
    {
        return x * x + y * y + z * z;
    }
    double magnitude() const
    {
        return std::sqrt(magnitute_squared());
    }
};

using Point3 = Vec3;
using color = Vec3;

// commutative law
inline Vec3 operator*(const double scalar, const Vec3 &vec)
{
    return vec * scalar;
}
inline double dot_product(const Vec3 &a, const Vec3 &b)
{
    return a.x * b.x +
           a.y * b.y +
           a.z * b.z;
}
inline Vec3 cross_product(const Vec3 &a, const Vec3 &b)
{
    return Vec3(a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x);
}
