#pragma once

#include "utils.h"

#include <fstream>
#include <string>

struct Settings
{
    double      x_space_per_sphere;
    double      y_space_per_sphere;
    double      radius;
    double      z;
    int         materials_amount;
    std::string text;
};

inline double get_double_from_line(std::istream& input)
{
    std::string input_buffer;
    std::getline(input, input_buffer, '=');
    std::getline(input, input_buffer);
    return std::stod(input_buffer);
}
inline int get_int_from_line(std::istream& input)
{
    std::string input_buffer;
    std::getline(input, input_buffer, '=');
    std::getline(input, input_buffer);
    return std::stoi(input_buffer);
}

inline void read_settings_file(std::string file_path, Settings& settings)
{
    try
    {
        std::fstream file;
        file.open(file_path);
        if (!file)
            raise_error("Can't open settings file " << file_path << "!");

        settings.x_space_per_sphere = get_double_from_line(file);
        settings.y_space_per_sphere = get_double_from_line(file);
        settings.radius             = get_double_from_line(file);
        settings.z                  = get_double_from_line(file);
        settings.materials_amount   = get_int_from_line(file);

        // rest is text
        std::ostringstream text_ss;
        text_ss << file.rdbuf();
        settings.text = text_ss.str();
        file.close();
    }
    catch (const std::exception& ex)
    {
        raise_error(ex.what() << " error parsing settings file " << file_path << "!");
    }
}
