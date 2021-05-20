#pragma once

#include "ray_tracer.h"

class Camera
{
private:
    Point3 m_origin;
    Point3 m_lower_left_corner;
    Vec3   m_horizontal;
    Vec3   m_vertical;
    Vec3   m_u, m_v, m_w;
    double m_lens_radius;

public:
    // vfov: vertical field-of-view in degrees
    // vup: view up
    Camera(Point3 lookfrom, Point3 lookat, Vec3 vup, double vfov, double aspect_ratio, double aperture, double focus_dist)
    {
        double theta = degrees_to_radians(vfov);
        // half viewport height
        double h               = tan(theta / 2);
        double viewport_height = 2.0 * h;
        double viewport_width  = aspect_ratio * viewport_height;

        // camera looks in -w direction
        m_w = unit_vector(lookfrom - lookat);
        // side axis
        m_u = unit_vector(cross_product(vup, m_w));
        // up axis
        m_v = cross_product(m_w, m_u);

        m_origin = lookfrom;
        // move viewplane into space (focus_dist deep)
        m_horizontal        = focus_dist * viewport_width * m_u;
        m_vertical          = focus_dist * viewport_height * m_v;
        m_lower_left_corner = m_origin - m_horizontal / 2 - m_vertical / 2 - focus_dist * m_w;

        m_lens_radius = aperture / 2;
    }

    Ray get_ray(double s, double t) const
    {
        // get random ray <- blurr
        Vec3 random_vec = m_lens_radius * random_in_unit_disk();
        Vec3 offset     = m_u * random_vec.x + m_v * random_vec.y;

        // from origin; to lower left corner of viewport but to the right and up
        // to current pixel
        return Ray(m_origin + offset,
                   m_lower_left_corner + s * m_horizontal + t * m_vertical - m_origin - offset);
    }
};
