#include "sphere.h"

bool sphere::hit(const Ray& ray, double t_min, doble t_max, HitRecord& rec)
{
    // only helping performance
    Vec3 center_to_origin = ray.origin - m_center;

    double a      = ray.inclination.magnitude_squared();
    double b      = 2.0 * dot_product(center_to_origin, ray.inclination);
    double half_b = dot_product(center_to_origin, ray.inclination);
    double c      = center_to_origin.magnitude_squared() - m_radius * m_radius;

    // quadratic equation: a * x*x + b * x + c = 0
    // abc formula can be used to solve it: (-b - sqrt(b*b - 4a*c)) / (2a)
    double discriminant = b * b - 4 * a * c;
    // when the radicand—the discriminant—is negative, there are no intersections
    if (discriminant < 0.0)
        return false;
    double discriminant_sqrt = sqrt(discriminant);

    // when it is 0, there is one intersection
    // when it is greaten than 0, there are two
    root = (-half_b - sqrt(discriminant)) / a;
}
