#include "ray_tracer.h"

#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "read_scene_file.h"
#include "scenes.h"
#include "sphere.h"

Color ray_color(const Ray& ray, const Hittable& world, int depth)
{
    HitRecord record;

    // no more light
    if (depth == 0)
        return Color(0.0, 0.0, 0.0);

    // fixing shadow acne
    if (world.hit(ray, 0.001, infinity, record))
    {
        Ray   scattered;
        Color attenuation;
        // does hit material not absorb ray
        if (record.material->scatter(ray, record, attenuation, scattered))
            // shoot reflected ray
            return attenuation * ray_color(scattered, world, depth - 1);
        // absorb ray
        return Color(0.0, 0.0, 0.0);
    }

    // or background
    // fyi: ray only touches corners of viewport; penetrates
    // middle -> gradient is not horizontally constant -> you can see a
    // curvature in the background
    Vec3 direction = unit_vector(ray.inclination);
    // from 0.0 to 1.0
    //                                         <- half the height of the viewport
    double normalized_y = 0.5 * (direction.y + 1.0);
    // linear blend between white and blue
    return (1.0 - normalized_y) * Color(1.0, 1.0, 1.0) +
           normalized_y * Color(0.5, 0.7, 1.0);
}

int main(int argc, char* argv[])
{
    // load scene
    Scene scene;
    read_scene_file(argv[1], scene);
    Camera cam = scene.get_cam();

    // no alpha
    constexpr int        channel_count = 3;
    std::vector<uint8_t> pixels;
    pixels.reserve(scene.image_width * scene.image_height * channel_count);

    // render
    // x and y of result image
    for (int y = scene.image_height - 1; y >= 0; --y)
    {
        for (int x = 0; x < scene.image_width; ++x)
        {
            // anti-aliasing: keep adding on this pixel; divide in end
            Color pixel_color(0.0, 0.0, 0.0);
            for (int i = 0; i < scene.samples_per_pixel; ++i)
            {
                // create current ray
                double normalized_x = (x + random_double()) / (scene.image_width - 1);
                double normalized_y = (y + random_double()) / (scene.image_height - 1);
                Ray    ray          = cam.get_ray(normalized_x, normalized_y);
                pixel_color += ray_color(ray, scene.world, scene.max_depth);
            }
            write_color(pixels, pixel_color, scene.samples_per_pixel);
        }
        std::cout << "\rScanlines remaining: " << y << ' ' << std::flush;
    }
    std::cout << "\nSaving file..." << std::endl;

    stbi_write_png("out.png", scene.image_width, scene.image_height, channel_count,
                   pixels.data(), scene.image_width * channel_count);
    std::cout << "All done; enjoy!" << std::endl;

    return 0;
}
