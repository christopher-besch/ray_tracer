#pragma once

#include "ray_tracer.h"

#include <string>

class Material;

// representation of last hit of one ray
struct HitRecord
{
    Point3    hit_point;
    Vec3      normal;
    Material* material;
    // distance to camera
    double distance;
    bool   front_face;

    // is ray outside object?
    inline void set_face_normal(const Ray& ray, const Vec3& outward_normal)
    {
        front_face = dot_product(ray.inclination, outward_normal) < 0;
        normal     = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable
{
public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const = 0;

    virtual std::string to_str() const = 0;
};
