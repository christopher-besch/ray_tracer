#include "random.h"
#include "ray_tracer.h"

#include <iostream>
#include <memory>
#include <sstream>
#include <thread>
#include <vector>
#ifndef WINDOWS
#include <pthread.h>
#endif
#include <mutex>

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

Color ray_color(const Ray& ray, const Hittable& world, int depth, RandomGen& random_gen)
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
        if (record.material->scatter(ray, record, attenuation, scattered, random_gen))
            // shoot reflected ray
            return attenuation * ray_color(scattered, world, depth - 1, random_gen);
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

class StatusWatcher
{
private:
    int        m_scanlines_remaining;
    std::mutex m_print_lock;

public:
    StatusWatcher(int scanlines)
        : m_scanlines_remaining(scanlines) {}

    // thread safe
    void scanline_done()
    {
        auto lock = std::unique_lock<std::mutex>(m_print_lock);
        std::cout << "\rScanlines remaining: " << --m_scanlines_remaining << ' ' << std::flush;
    }
};

void render_scanlines(const Scene& scene, const Camera& cam, size_t offset, size_t length, std::vector<uint8_t>& pixels, StatusWatcher& status_watcher)
{
    RandomGen random_gen;
    // x and y of result image
    for (int y = offset; y < offset + length; ++y)
    {
        for (int x = 0; x < scene.image_width; ++x)
        {
            // anti-aliasing: keep adding on this pixel; divide in end
            Color pixel_color(0.0, 0.0, 0.0);
            for (int i = 0; i < scene.samples_per_pixel; ++i)
            {
                // create current ray
                double normalized_x = (x + random_gen.next_double()) / (scene.image_width - 1);
                double normalized_y = (y + random_gen.next_double()) / (scene.image_height - 1);
                Ray    ray          = cam.get_ray(normalized_x, normalized_y, random_gen);
                pixel_color += ray_color(ray, scene.world, scene.max_depth, random_gen);
            }
            write_color(pixels, pixel_color, scene.samples_per_pixel, (scene.image_height - 1 - y) * scene.image_width * 3 + x * 3);
        }
        status_watcher.scanline_done();
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2)
        raise_error("Please specify the input scene file as the first console parameter.");
    int thread_amount = 1;
    if (argc > 2)
        thread_amount = checked_stoi(argv[2]);
    if (thread_amount <= 0)
        raise_error("Dude please, you know I need some threads to play with.");

    // load scene
    Scene scene;
    read_scene_file(argv[1], scene);
    Camera cam = scene.get_cam();

    // no alpha
    constexpr int        channel_count = 3;
    std::vector<uint8_t> pixels(scene.image_width * scene.image_height * channel_count);

    // render multi threaded
    StatusWatcher status_watcher(scene.image_height);

    std::vector<std::thread> thread_pool;
    thread_pool.resize(thread_amount);
    int scanlines_per_thread = scene.image_height / thread_amount;
    // hand rest scanlines evenly to threads
    int scanlines_rest      = scene.image_height % thread_amount;
    int scanlines_rest_left = scanlines_rest;
    for (int i = 0; i < thread_amount; ++i)
    {
        size_t offset = i * scanlines_per_thread + scanlines_rest - scanlines_rest_left;
        size_t length;
        // when there are rest scanlines left to be assigned
        if (scanlines_rest_left)
        {
            --scanlines_rest_left;
            length = scanlines_per_thread + 1;
        }
        else
            length = scanlines_per_thread;
        thread_pool[i] = std::thread(&render_scanlines, std::ref(scene), std::ref(cam), offset, length, std::ref(pixels), std::ref(status_watcher));
    }

    for (std::thread& thread : thread_pool)
        thread.join();
    std::cout << "\nSaving file..." << std::endl;

    stbi_write_png("out.png", scene.image_width, scene.image_height, channel_count,
                   pixels.data(), scene.image_width * channel_count);
    std::cout << "All done; enjoy!" << std::endl;

    return 0;
}
