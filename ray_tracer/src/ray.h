#pragma once

#include "vec3.h"

struct Ray
{
    Point3 origin;
    Vec3   inclination;

    Ray() {}
    Ray(const Point3& origin, const Vec3& inclination)
        : origin(origin), inclination(inclination) {}

    Point3 at(double lambda) const
    {
        return origin + lambda * inclination;
    }
};
