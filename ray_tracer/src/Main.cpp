#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "Vector.h"

int main()
{
    // let's be friendly to the compiler
    constexpr int width = 200;
    constexpr int height = 100;
    // no alpha
    constexpr int channel_count = 3;

    // uint8_t *pixels = new uint8_t[width * height * channel_num];
    std::unique_ptr<uint8_t[]> pixels = std::make_unique<uint8_t[]>(width * height * channel_count);

    int index = 0;
    for (float y = height - 1; y >= 0; --y)
    {
        for (float x = 0; x < width; ++x)
        {
            float r = x / width;
            float g = y / height;
            float b = 0.2f;
            uint8_t ir = ceil(255.0f * r);
            uint8_t ig = ceil(255.0f * g);
            uint8_t ib = ceil(255.0f * b);

            pixels[index++] = ir;
            pixels[index++] = ig;
            pixels[index++] = ib;
        }
    }

    // stbi_write_png("out.png", width, height, channel_num, reinterpret_cast<const void *>(&pixels[0]), width * channel_num);
    stbi_write_png("out.png", width, height, channel_count, pixels.get(), width * channel_count);

    Vec3 a(1, 2, 3);

    std::cout << a[1] << std::endl;

    return 0;
}
