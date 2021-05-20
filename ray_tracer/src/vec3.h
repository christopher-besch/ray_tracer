#pragma once

#include <cmath>

#include "random.h"

struct Vec3
{
    // that's also a union
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
        : e { 0, 0, 0 } {}
    Vec3(double x, double y, double z)
        : e { x, y, z } {}

    Vec3 operator-() const
    {
        return Vec3({ -x, -y, -z });
    }
    double operator[](int i) const
    {
        return e[i];
    }
    double& operator[](int i)
    {
        return e[i];
    }

    // utility functions
    Vec3 operator+(const Vec3& other) const
    {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }
    Vec3 operator-(const Vec3& other) const
    {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }
    Vec3 operator*(const Vec3& other) const
    {
        return Vec3(x * other.x, y * other.y, z * other.z);
    }
    Vec3 operator*(double scalar) const
    {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }
    Vec3 operator/(double scalar) const
    {
        return *this * (1 / scalar);
    }

    Vec3& operator+=(const Vec3& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    Vec3& operator-=(const Vec3& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    Vec3& operator*=(double scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    Vec3& operator/=(double num)
    {
        x *= 1 / num;
        y *= 1 / num;
        z *= 1 / num;
        return *this;
    }

    double magnitude_squared() const
    {
        return x * x + y * y + z * z;
    }
    double magnitude() const
    {
        return std::sqrt(magnitude_squared());
    }

    // if close to zero in all dimensions
    bool near_zero() const
    {
        const double s = 1e-8;
        return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
    }
};

using Point3 = Vec3;
using Color  = Vec3;

// commutative law
inline Vec3 operator*(double scalar, const Vec3& vec)
{
    return vec * scalar;
}
inline double dot_product(const Vec3& a, const Vec3& b)
{
    return a.x * b.x +
           a.y * b.y +
           a.z * b.z;
}
inline Vec3 cross_product(const Vec3& a, const Vec3& b)
{
    return Vec3(a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x);
}

///////////////////////
// vector generators //
///////////////////////
inline Vec3 unit_vector(Vec3 vec)
{
    return vec / vec.magnitude();
}

inline Vec3 random_vector()
{
    return Vec3(random_double(), random_double(), random_double());
}

inline Vec3 random_vector(double min, double max)
{
    return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

inline Vec3 random_in_unit_sphere()
{
    while (true)
    {
        Vec3 point = random_vector(-1, 1);
        if (point.magnitude_squared() >= 1) continue;
        return point;
    }
}

// on surface of unit sphere
inline Vec3 random_unit_vector()
{
    return unit_vector(random_in_unit_sphere());
}

inline Vec3 random_in_hemisphere(const Vec3& normal)
{
    Vec3 in_unit_sphere = random_in_unit_sphere();
    // in same hemisphere as normal
    if (dot_product(in_unit_sphere, normal) > 0.0)
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

inline Vec3 random_in_unit_disk()
{
    while (true)
    {
        Vec3 p(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.magnitude_squared() >= 1.0) continue;
        return p;
    }
}

// reflect <incoming> off of surface with normal <normal>
inline Vec3 reflect(const Vec3& incoming, const Vec3& normal)
{
    // scale normal with twice depth of virtual penetration
    return incoming - 2 * dot_product(incoming, normal) * normal;
}

// return refracted vector according to incidence and transmission indices of refraction
inline Vec3 refract(const Vec3& in, const Vec3& normal, double eta_in_over_eta_trans)
{
    double cos_theta      = fmin(dot_product(-in, normal), 1.0);
    Vec3   trans_perp     = eta_in_over_eta_trans * (in + cos_theta * normal);
    Vec3   trans_parallel = -sqrt(fabs(1.0 - trans_perp.magnitude_squared())) * normal;
    return trans_perp + trans_parallel;
}
