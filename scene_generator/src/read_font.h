#pragma once

#include "utils.h"

#include <fstream>
#include <map>
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

inline void read_font(std::string file_path, Font& font)
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
