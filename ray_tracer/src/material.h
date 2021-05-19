#pragma once

#include "hittable.h"
#include "ray_tracer.h"
#include "vec3.h"

class Material
{
public:
    // does not get absorbed?
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
#if 1
        // trace reflected ray from within sphere at normal, kissing last hit
        Vec3 scatter_direction = record.normal + random_unit_vector();
#else
        // trace reflected ray from within hemisphere of normal
        Vec3 scatter_direction = random_in_hemisphere(record.normal);
#endif

        // when unit vector opposite normal -> zero vector bad
        if (scatter_direction.near_zero())
            scatter_direction = record.normal;

        scattered   = Ray(record.hit_point, scatter_direction);
        attenuation = m_albedo;
        return true;
    }
};

class Metal : public Material
{
private:
    Color  m_albedo;
    double m_fuzziness;

public:
    Metal(const Color& albedo, double fuzziness)
        : m_albedo(albedo), m_fuzziness(fuzziness) {}

    virtual bool scatter(const Ray& ray_in, const HitRecord& record, Color& attenuation, Ray& scattered) const override
    {
        Vec3 reflected = reflect(unit_vector(ray_in.inclination), record.normal);
        // build reflected ray
        // the fuzzier, the bigger the deviaton sphere
        scattered   = Ray(record.hit_point, reflected + m_fuzziness * random_in_unit_sphere());
        attenuation = m_albedo;
        // reflect only when reflection in hemisphere of normal
        return dot_product(scattered.inclination, record.normal) > 0;
    }
};

class Dielectric : public Material
{
private:
    // index of refraction
    double m_ior;

public:
    Dielectric(double index_of_refrection)
        : m_ior(index_of_refrection) {}

    virtual bool scatter(const Ray& ray_in, const HitRecord& record, Color& attenuation, Ray& scattered) const override
    {
        attenuation = Color(1.0, 1.0, 1.0);
        // ior of air is 0
        // looking at sphere?
        // incident ray; transmission ray
        // todo: store ior in ray
        double eta_in_over_eta_trans = record.front_face ? (1.0 / m_ior) : m_ior;

        Vec3   unit_direction = unit_vector(ray_in.inclination);
        double cos_theta      = fmin(dot_product(-unit_direction, record.normal), 1.0);
        double sin_theta      = sqrt(1.0 - cos_theta * cos_theta);

        // cannot refract <- snell's law unsolvable with e.g. sin(1.2)
        bool cannot_refract = eta_in_over_eta_trans * sin_theta > 1.0;
        Vec3 direction;
        // steep angles are more reflective
        if (cannot_refract || reflectance(cos_theta, eta_in_over_eta_trans) > random_double())
            direction = reflect(unit_direction, record.normal);
        else
            direction = refract(unit_direction, record.normal, eta_in_over_eta_trans);

        scattered = Ray(record.hit_point, direction);
        return true;
    }

private:
    // use schlick's approximation
    // ugly polynomial ...
    static double reflectance(double cosine, double eta_in_over_eta_trans)
    {
        double r0 = (1 - eta_in_over_eta_trans) / (1 + eta_in_over_eta_trans);
        r0        = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};
