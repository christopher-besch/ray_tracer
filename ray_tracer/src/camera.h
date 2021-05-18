#pragma once

#include "ray_tracer.h"

class Camera
{
private:
    Point3 m_origin;
    Point3 m_lower_left_corner;
    Vec3   m_horizontal;
    Vec3   m_vertical;

public:
    Camera()
    {
        // axis:
        //  x: to the right
        //  y: up
        //  z: to behind camera
        double aspect_ratio    = 16.0 / 9.0;
        double viewport_height = 2.0;
        double viewport_width  = aspect_ratio * viewport_height;
        double focal_length    = 1.0;

        m_origin            = Point3(0.0, 0.0, 0.0);
        m_horizontal        = Vec3(viewport_width, 0.0, 0.0);
        m_vertical          = Vec3(0.0, viewport_height, 0.0);
        m_lower_left_corner = m_origin - m_horizontal / 2 - m_vertical / 2 - Vec3(0, 0, focal_length);
    }

    Ray get_ray(double x, double y) const
    {
        // from origin; to lower left corner of viewport but to the right and up
        // to current pixel fyi: ray only touches corners of viewport; penetrates
        // middle -> gradient is not horizontally constant -> you can see a
        // curvature in the background
        return Ray(m_origin, m_lower_left_corner + x * m_horizontal + y * m_vertical - m_origin);
    }
};
