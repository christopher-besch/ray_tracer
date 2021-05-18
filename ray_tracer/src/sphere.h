#pragma once

#include "hittable.h"
#include "vec3.h"

class Sphere : public Hittable
{
private:
    Point3 m_center;
    double m_radius;

public:
    Sphere() {}
    Sphere(Point3 center, double radius)
        : m_center(center), m_radius(radius) {};

    virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const override;
};
