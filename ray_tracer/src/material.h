#pragma once

#include "hittable.h"
#include "ray_tracer.h"

class Material
{
public:
    virtual bool scatter(const Ray& ray_in, const HitRecord& record, Color& attenuation, Ray& scattered) const = 0;
};
