#include "sphere.h"

bool Sphere::hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const
{
    // only helping performance
    Vec3 center_to_origin = ray.origin - m_center;

    double a = ray.inclination.magnitude_squared();
    // optimization
    double half_b = dot_product(center_to_origin, ray.inclination);
    double c      = center_to_origin.magnitude_squared() - m_radius * m_radius;

    // quadratic equation: a * x*x + b * x + c = 0
    // abc formula can be used to solve it: (-b - sqrt(b*b - 4a*c)) / (2a)
    double discriminant = half_b * half_b - a * c;
    // when the radicand—the discriminant—is negative, there are no intersections
    if (discriminant < 0.0)
        return false;
    double discriminant_sqrt = sqrt(discriminant);

    // when it is 0, there is one intersection
    // when it is greaten than 0, there are two
    // find nearest root in acceptable range
    double root = (-half_b - discriminant_sqrt) / a;
    if (root < t_min || t_max < root)
    {
        root = (-half_b + discriminant_sqrt) / a;
        if (root < t_min || t_max < root)
            // no root in range found
            return false;
    }

    // store point
    record.distance  = root;
    record.hit_point = ray.at(record.distance);
    // store normal
    Vec3 outward_normal = (record.hit_point - m_center) / m_radius;
    record.set_face_normal(ray, outward_normal);
    return true;
}
