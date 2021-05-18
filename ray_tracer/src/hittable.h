#pragma once

#include "ray.h"

// representation of last hit of one ray
struct HitRecord
{
    Point3 hit_point;
    Vec3   normal;
    // distance to camera
    double distance;
    bool   front_face;

    inline void set_face_normal(const Ray& ray, const Vec3& outward_normal)
    {
        // is ray outside object?
        front_face = dot_product(ray.inclination, outward_normal) < 0;
        normal     = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable
{
public:
    virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const = 0;
};
