#pragma once

#include "hittable.h"
#include "vec3.h"
#include <sstream>

class Sphere : public Hittable
{
private:
    Point3    m_center;
    double    m_radius;
    Material* m_material;

public:
    Sphere() {}
    Sphere(Point3 center, double radius, Material* material)
        : m_center(center), m_radius(radius), m_material(material) {};

    virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const override;

    virtual std::string to_str() const override
    {
        std::stringstream buffer;
        buffer << "<Sphere c:" << m_center << ", r: " << m_radius << ">";
        return buffer.str();
    }
};
