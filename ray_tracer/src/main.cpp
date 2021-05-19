#include "ray_tracer.h"

#include <fstream>
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
        if (record.material->scatter(ray, record, attenuation, scattered))
            // shoot reflected ray
            return attenuation * ray_color(scattered, world, depth - 1);
        // absorb ray
        return Color(0.0, 0.0, 0.0);
    }

    // or background
    Vec3 direction = unit_vector(ray.inclination);
    // from 0.0 to 1.0
    //                                         <- half the height of the viewport
    double normalized_y = 0.5 * (direction.y + 1.0);
    // linear blend between white and blue
    return (1.0 - normalized_y) * Color(1.0, 1.0, 1.0) +
           normalized_y * Color(0.5, 0.7, 1.0);
}

int main()
{
    ///////////
    // image //
    ///////////
    constexpr double aspect_ratio = 16.0 / 9.0;
    constexpr long   image_height = 420;
    constexpr long   image_width  = static_cast<int>(image_height * aspect_ratio);
    // no alpha
    constexpr int channel_count     = 3;
    constexpr int samples_per_pixel = 50;
    constexpr int max_depth         = 5;

    std::vector<uint8_t> pixels;
    pixels.reserve(image_width * image_height * channel_count);

    ///////////
    // world //
    ///////////
    HittableList world;

    std::shared_ptr<Material> material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    std::shared_ptr<Material> material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    std::shared_ptr<Material> material_left   = std::make_shared<Dielectric>(1.0);
    std::shared_ptr<Material> material_right  = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);

    world.add(std::make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(std::make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.4, material_left));
    world.add(std::make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));


    ////////////
    // render //
    ////////////
    Camera cam;
    // x and y of result image
    for (int y = image_height - 1; y >= 0; --y)
    {
        for (int x = 0; x < image_width; ++x)
        {
            // keep adding on this pixel; divide in end
            Color pixel_color(0.0, 0.0, 0.0);
            for (int i = 0; i < samples_per_pixel; ++i)
            {
                // create current ray
                double normalized_x = (x + random_double()) / (image_width - 1);
                double normalized_y = (y + random_double()) / (image_height - 1);
                Ray    ray          = cam.get_ray(normalized_x, normalized_y);
                pixel_color += ray_color(ray, world, max_depth);
            }
            write_color(pixels, pixel_color, samples_per_pixel);
        }
        std::cout << "\rScanlines remaining: " << y << ' ' << std::flush;
    }
    std::cout << "\nSaving file..." << std::endl;

    // stbi_write_png("out.png", width, height, channel_num,
    // reinterpret_cast<const void *>(&pixels[0]), width * channel_num);
    stbi_write_png("out.png", image_width, image_height, channel_count,
                   pixels.data(), image_width * channel_count);
    std::cout << "All done; enjoy!" << std::endl;

    return 0;
}
