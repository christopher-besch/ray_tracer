#pragma once

#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <filesystem>

#ifdef LINUX
#define file_slash '/'
#endif
#ifdef WINDOWS
#define file_slash '\\'
#endif

// load files from cwd or from execution location
#ifdef IDE
inline std::string get_virtual_cwd(std::string execution_path)
{
    return "";
}
#else
inline std::string get_virtual_cwd(std::string execution_path)
{
    // cut at last slash
    std::string virtual_cwd = "";
    for (int idx = execution_path.size() - 1; idx >= 0; idx--)
        if (execution_path[idx] == file_slash)
        {
            virtual_cwd = execution_path.substr(0, idx + 1);
            break;
        }
    return virtual_cwd;
}
#endif

#ifdef DEBUG
#define raise_error(msg)                                                                                                  \
    {                                                                                                                     \
        std::cerr << msg << " (in: " << __FILE__ << ":" << __LINE__ << "; in function: " << __func__ << ")" << std::endl; \
        std::exit(EXIT_FAILURE);                                                                                          \
    }
#else
#define raise_error(msg)               \
    {                                  \
        std::cerr << msg << std::endl; \
        std::exit(EXIT_FAILURE);       \
    }
#endif

inline int checked_stoi(std::string str)
{
    try
    {
        return std::stoi(str);
    }
    catch (const std::invalid_argument ex)
    {
        raise_error("Can't convert \"" << str << "\" to int!");
    }
}

inline void remove_chars(std::string &str, std::vector<char> chars)
{
    for (int idx = str.size() - 1; idx >= 0; idx--)
        if (std::find(chars.begin(), chars.end(), str[idx]) != chars.end())
            str.erase(idx, 1);
}
