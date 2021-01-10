// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <memory>
//
// #define STB_IMAGE_IMPLEMENTATION
// #include <stb_image.h>
//
// #define STBI_MSC_SECURE_CRT
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include <stb_image_write.h>
//
// int main()
// {
//     const int width = 200;
//     const int height = 100;
//     // no alpha
//     const int channel_num = 3;
//
//     std::unique_ptr<uint8_t[]> pixels = std::make_unique<uint8_t[]>(width * height * channel_num) > ;
//
//     int index = 0;
//     for (int j = height - 1; j >= 0; --j)
//     {
//         for (int i = 0; i < width; ++i)
//         {
//             float r = (float)i / (float)width;
//             float g = (float)j / (float)height;
//             float b = 0.2f;
//             int ir = int(255.99 * r);
//             int ig = int(255.99 * g);
//             int ib = int(255.99 * b);
//
//             pixels[index++] = ir;
//             pixels[index++] = ig;
//             pixels[index++] = ib;
//         }
//     }
//
//     stbi_write_png("out.png", width, height, channel_num, reinterpret_cast<const void *>(pixels), width * channel_num);
//
//     return 0;
// }

#include <iostream>
#include <SDL.h>

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << "SDL initialization failed. SDL Error: " << SDL_GetError();
    }
    else
    {
        std::cout << "SDL initialization succeeded!";
    }

    return 0;
}

/*
project "SDL"
    language "C"
    architecture "x86_64"
    kind "StaticLib"

    location ""
    targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

    -- all files that are included or compiled = all files that are visible to the preprocessor/compiler/linker and make and stuff
    files
    {
        "src/**.h",
        "include/**.c"
    }

    -- for #include paths with ""
    includedirs {
        "src",
        "include"
    }

*/
