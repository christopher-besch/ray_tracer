#pragma once

#include "ray.h"

struct HitRecord
{
    Point3 p;
    Vec3   normal;
    double t;
};

class Hittable
{
public:
    virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const = 0;
};
