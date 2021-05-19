#pragma once

#include "hittable.h"
#include "ray_tracer.h"
#include "vec3.h"

class Material
{
public:
    virtual bool scatter(const Ray& ray_in, const HitRecord& record, Color& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material
{
private:
    Color m_albedo;

public:
    Lambertian(const Color& albedo)
        : m_albedo(albedo) {}

    virtual bool scatter(const Ray& ray_in, const HitRecord& record, Color& attenuation, Ray& scattered) const override
    {
        Vec3 scatter_direction = record.normal + random_unit_vector();

        // when unit vector opposite normal -> zero vector bad
        if (scatter_direction.near_zero())
            scatter_direction = record.normal;

        scattered   = Ray(record.hit_point, scatter_direction);
        attenuation = m_albedo;
        return true;
    }
}
