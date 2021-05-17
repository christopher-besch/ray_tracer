#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "color.h"
#include "ray.h"
#include "vec3.h"

double hit_sphere(const Point3& center, double radius, const Ray& ray)
{
    // only helping performance
    Vec3 center_to_origin = ray.origin - center;

    double a      = ray.inclination.magnitude_squared();
    double b      = 2.0 * dot_product(center_to_origin, ray.inclination);
    double half_b = dot_product(center_to_origin, ray.inclination);
    double c      = center_to_origin.magnitude_squared() - radius * radius;

    // quadratic equation: a * x*x + b * x + c = 0
    // abc formula can be used to solve it: (-b - sqrt(b*b - 4a*c)) / (2a)
    double discriminant = b * b - 4 * a * c;
    // when the radicand—the discriminant—is negative, there are no intersections
    if (discriminant < 0.0)
        return -1.0;
    // when it is 0, there is one intersection
    // when it is greaten than 0, there are two
    // return closest hit point
    return (-half_b - sqrt(discriminant)) / a;
}

Color get_background_color(const Ray& ray)
{
    Vec3 direction = unit_vector(ray.inclination);
    // from 0.0 to 1.0
    //                                         <- half the height of the viewport
    double normalized_y = 0.5 * (direction.y + 1.0);
    // linear blend between white and blue
    return (1.0 - normalized_y) * Color(1.0, 1.0, 1.0) +
           normalized_y * Color(0.5, 0.7, 1.0);
}

Color ray_color(const Ray& ray)
{
    double hit_point = hit_sphere(Point3(0.0, 0.0, -1.0), 0.5, ray);
    // hit sphere?
    if (hit_point > 0.0)
    {
        Vec3 normal = unit_vector(ray.at(hit_point) - Vec3(0, 0, -1));
        return 0.5 * Color(normal.x + 1, normal.y + 1, normal.z + 1);
    }
    return get_background_color(ray);
}

int main()
{
    // image setup
    constexpr double aspect_ratio = 16.0 / 9.0;
    constexpr long   image_height = 1080;
    constexpr long   image_width  = static_cast<int>(image_height * aspect_ratio);
    // no alpha
    constexpr int channel_count = 3;

    // camera setup
    // axis:
    //  x: to the right
    //  y: up
    //  z: to behind camera
    double viewport_height = 2.0;
    double viewport_width  = viewport_height * aspect_ratio;
    double focal_length    = 1.0;

    // camera
    Point3 origin(0.0, 0.0, 0.0);
    Vec3   horizontal(viewport_width, 0.0, 0.0);
    Vec3   vertical(0.0, viewport_height, 0.0);
    // go to left side of viewport; go to lower side; go focal length into space
    Point3 lower_left_corner =
        origin - horizontal / 2 - vertical / 2 - Vec3(0.0, 0.0, focal_length);

    // uint8_t *pixels = new uint8_t[width * height * channel_num];
    std::unique_ptr<uint8_t[]> pixels =
        std::make_unique<uint8_t[]>(image_width * image_height * channel_count);

    int index = 0;
    // x and y of result image
    for (int y = image_height - 1; y >= 0; --y)
    {
        for (int x = 0; x < image_width; ++x)
        {
            // create current ray
            double normalized_x = static_cast<double>(x) / (image_width - 1);
            double normalized_y = static_cast<double>(y) / (image_height - 1);
            // from origin; to lower left corner of viewport but to the right and up
            // to current pixel fyi: ray only touches corners of viewport; penetrates
            // middle -> gradient is not horizontally constant -> you can see a
            // curvature in the background
            Ray   ray(origin, lower_left_corner + normalized_x * horizontal +
                                normalized_y * vertical - origin);
            Color pixel = ray_color(ray);
            write_color(pixels, index, pixel);
        }
        std::cout << "\rScanlines remaining: " << y << ' ' << std::flush;
    }
    std::cout << "\nSaving file..." << std::endl;

    // stbi_write_png("out.png", width, height, channel_num,
    // reinterpret_cast<const void *>(&pixels[0]), width * channel_num);
    stbi_write_png("out.png", image_width, image_height, channel_count,
                   pixels.get(), image_width * channel_count);
    std::cout << "All done; enjoy!" << std::endl;

    return 0;
}
