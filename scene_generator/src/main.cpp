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

int main(int argc, char* argv[])
{
    std::string file_path = std::string("fonts") + file_slash + "block.txt";
    Font        font;
    read_font(file_path, font);
    std::string                    text = argv[1];
    std::vector<std::vector<bool>> pixel_lines;
    pre_render(text, pixel_lines, font);

    for (std::vector<bool> line : pixel_lines)
    {
        for (int i = 0; i < line.size(); ++i)
        {
            if (i % 2)
                std::cout << (line[i] ? '#' : ' ');
        }
        std::cout << std::endl;
    }
}
