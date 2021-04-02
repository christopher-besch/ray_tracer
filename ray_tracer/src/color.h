#pragma once

#include <memory>

#include "vec3.h"

void write_color(std::unique_ptr<uint8_t[]> &pixels, int &index, Vec3 &pixel)
{
    // normalize
    uint8_t ir = ceil(255.0f * pixel.r);
    uint8_t ig = ceil(255.0f * pixel.g);
    uint8_t ib = ceil(255.0f * pixel.b);
    pixels[index++] = ir;
    pixels[index++] = ig;
    pixels[index++] = ib;
}
