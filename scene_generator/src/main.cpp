#include "random.h"
#include "utils.h"

#include "read_font.h"
#include "read_settings.h"

#include <bits/stdint-uintn.h>
#include <iostream>
#include <sstream>

// turn single line with font in list of pixels
void pre_render_line(const std::string& line, std::vector<std::vector<bool>>& pixel_lines, const Font& font)
{
    // create pixel lines
    pixel_lines.resize(font.height);
    for (char code : line)
    {
        Character character = font.characters.at(code);
        for (int i = 0; i < font.height; ++i)
        {
            // push single line of character pixels to pixel line
            pixel_lines[i].insert(pixel_lines[i].end(), character.pixels.begin() + i * character.width, character.pixels.begin() + (i + 1) * character.width);
            pixel_lines[i].insert(pixel_lines[i].end(), font.spacing, false);
        }
    }
}

// turn text with font in list of pixels
void pre_render(const std::string& text, std::vector<std::vector<bool>>& pixel_lines, const Font& font)
{
    std::string       input_buffer;
    std::stringstream text_ss(text);
    // add row after row to pixels
    while (std::getline(text_ss, input_buffer))
    {
        std::vector<std::vector<bool>> line_pixels;
        pre_render_line(input_buffer, line_pixels, font);
        // add newly pre rendered lines = one row
        pixel_lines.insert(pixel_lines.end(), line_pixels.begin(), line_pixels.end());
    }
}

void render(const std::vector<std::vector<bool>>& pixel_lines, std::string& output, const Settings& settings, RandomGen& random_gen)
{
    // append to output
    std::stringstream output_buffer(output);

    int width  = 0;
    int height = pixel_lines.size();
    for (int y = 0; y < height; ++y)
    {
        if (pixel_lines[y].size() > width)
            width = pixel_lines[y].size();
        for (int x = 0; x < pixel_lines[y].size(); ++x)
        {
            if (!(x % 2) && pixel_lines[y][x])
            {
                output_buffer << "s "
                              // take only second x-coordinates
                              << settings.x_space_per_sphere * x / 2 << " "
                              << settings.y_space_per_sphere * (height - 1 - y) << " "
                              << settings.z << " "
                              << settings.radius << " "
                              << random_gen.next_int(0, settings.materials_amount - 1) << std::endl;
            }
        }
    }
    std::cerr << "width: " << settings.x_space_per_sphere * width / 2
              << ", height: " << settings.y_space_per_sphere * height << std::endl;
    output = output_buffer.str();
}

int main(int argc, char* argv[])
{
    // load font
    std::string file_path = get_virtual_cwd(argv[0]) + std::string("fonts") + file_slash + "block.txt";
    Font        font;
    read_font(file_path, font);

    // read settings
    if (argc < 2)
        raise_error("Please specify the settings file as the first console parameter.");
    Settings settings;
    read_settings_file(argv[1], settings);

    // pre render
    std::vector<std::vector<bool>> pixel_lines;
    pre_render(settings.text, pixel_lines, font);

    // debug
    for (std::vector<bool> line : pixel_lines)
    {
        for (int i = 0; i < line.size(); ++i)
            if (i % 2)
                std::cerr << (line[i] ? '#' : ' ');
        std::cerr << std::endl;
    }
    // render
    std::string output;
    RandomGen   random_gen;
    render(pixel_lines, output, settings, random_gen);
    std::cout << output << std::endl;
    return 0;
}
