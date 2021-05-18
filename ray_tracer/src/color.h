#pragma once

#include "ray_tracer.h"

#include <iostream>
#include <memory>
#include <vector>

#include "vec3.h"

// write pixel to pixels and increase index
void write_color(std::vector<uint8_t>& pixels, Color pixel, int samples_per_pixel)
{
    pixel /= samples_per_pixel;

    // normalize
    // gamma correct for gamma=2.0
    const uint8_t r = 256 * clamp(sqrt(pixel.r), 0.0, 0.999);
    const uint8_t g = 256 * clamp(sqrt(pixel.g), 0.0, 0.999);
    const uint8_t b = 256 * clamp(sqrt(pixel.b), 0.0, 0.999);

    pixels.push_back(r);
    pixels.push_back(g);
    pixels.push_back(b);
}
