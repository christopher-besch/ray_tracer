#pragma once

#include "hittable.h"
#include "vec3.h"

class Sphere : public Hittable
{
private:
    Point3                    m_center;
    double                    m_radius;
    std::shared_ptr<Material> m_material;

public:
    Sphere() {}
    Sphere(Point3 center, double radius, std::shared_ptr<Material> material)
        : m_center(center), m_radius(radius), m_material(material) {};

    virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const override;
};
