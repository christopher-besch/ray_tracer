#include "utils.h"

#include <bits/stdint-uintn.h>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

struct Character
{
    int               width;
    std::vector<bool> pixels;
};

struct Font
{
    int height;
    int spacing;
    // 2d array line by line
    std::map<char, Character> characters;
};

void read_font(std::string file_path, Font& font)
{
    try
    {
        std::fstream file;
        file.open(file_path);
        if (!file)
            raise_error("Can't open font file " << file_path << "!");

        // read metadata
        std::string input_buffer;
        std::getline(file, input_buffer, '=');
        std::getline(file, input_buffer, '\n');
        font.height = std::stoi(input_buffer);

        std::getline(file, input_buffer, '=');
        std::getline(file, input_buffer, '\n');
        font.spacing = std::stoi(input_buffer);
        // skip comment line
        std::getline(file, input_buffer, '\n');

        // read characters
        while (std::getline(file, input_buffer, '-'))
        {
            // create new character
            std::getline(file, input_buffer, '\n');
            // ascii code
            char       code            = std::stoi(input_buffer);
            auto       character_match = font.characters.insert({ code, {} });
            Character& character       = character_match.first->second;

            bool first = true;
            // read lines of character
            for (int i = 0; i < font.height; ++i)
            {
                std::getline(file, input_buffer, '\n');
                // optimize
                if (first)
                {
                    character.width = input_buffer.size();
                    character.pixels.reserve(font.height * character.width);
                }
                else if (character.width != input_buffer.size())
                    raise_error("Incorrect width with char " << static_cast<int>(code) << " (" << code << ")");

                // convert string to list of bools
                for (char pixel : input_buffer)
                {
                    // anything that is not ' ' is a filled pixel
                    character.pixels.push_back(pixel != ' ');
                }
                first = false;
            }
        }
    }
    catch (const std::invalid_argument& ex)
    {
        raise_error(ex.what() << " error parsing font file " << file_path << "!");
    }
}

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

void render(const std::vector<std::vector<bool>>& pixel_lines, std::string& output)
{
    constexpr double x_space_per_sphere = 0.1;
    constexpr double y_space_per_sphere = 0.1;
    constexpr double radius             = 0.05;
    constexpr double z                  = -1.0;

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
                // todo: random material
                output_buffer << "s "
                              // take only second x-coordinates
                              << x_space_per_sphere * x / 2 << " "
                              << y_space_per_sphere * (height - 1 - y) << " "
                              << z << " "
                              << radius << " "
                              << "0" << std::endl;
            }
        }
    }
    std::cerr << "width: " << x_space_per_sphere * width / 2 << ", height: " << y_space_per_sphere * height << ", z:" << z << ", radius: " << radius << std::endl;
    output = output_buffer.str();
}

int main(int argc, char* argv[])
{
    // load font
    if (argc < 2)
        raise_error("Please specify the input text file as the first console parameter.");
    std::string file_path = get_virtual_cwd(argv[0]) + std::string("fonts") + file_slash + "block.txt";
    Font        font;
    read_font(file_path, font);

    // read text
    std::fstream file;
    file.open(argv[1]);
    if (!file)
        raise_error("Can't open input file " << argv[1] << "!");
    std::ostringstream text_ss;
    text_ss << file.rdbuf();
    std::string text = text_ss.str();

    // pre render
    std::vector<std::vector<bool>> pixel_lines;
    pre_render(text, pixel_lines, font);

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
    render(pixel_lines, output);
    std::cout << output << std::endl;
    return 0;
}
