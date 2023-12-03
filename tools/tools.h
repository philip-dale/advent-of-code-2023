#pragma once

#include <cstdint>
#include <iostream>
#include <iomanip>
#include <ios>
#include <sstream>
#include <string>
#include <vector>

#ifdef _WIN32
    #define NEW_LINE "\r\n"
#else
    #define NEW_LINE "\n"
#endif

std::string to_string(std::vector<std::uint8_t> const & data);
std::string to_hex_string(std::vector<std::uint8_t> const & data);
std::string file_to_string(std::string const& filename);

template<class T> 
T string_to(std::string const& str)
{
    if constexpr (std::is_same_v<T, std::string>)
    {
        return str;
    }
    std::istringstream convert(str);
    T value;
    convert >> value;
    return value;
}

template<class T> 
T string_to(std::string const& str, std::string const & sep)
{
    std::stringstream convert(str);
    T value{sep};
    convert >> value;
    return value;
}

template<class T>
std::vector<T> str_to_vec_sep(std::string const& data, std::string const & sub_sep, std::string const& separator=NEW_LINE)
{
    auto ret = std::vector<T>{};
    std::size_t last_pos = 0;
    auto pos = data.find(separator, last_pos);
    while(pos != std::string::npos)
    {
        ret.emplace_back(string_to<T>(data.substr(last_pos, pos-last_pos), sub_sep));
        last_pos = pos + separator.size();
        pos = data.find(separator, last_pos);
    }
    // Handle not having separator at end of a line
    if(last_pos < data.size())
    {
        ret.emplace_back(string_to<T>(data.substr(last_pos)));
    }
    return ret;
};

template<class T>
std::vector<T> str_to_vec(std::string const& data, std::string const& separator=NEW_LINE)
{
    auto ret = std::vector<T>{};
    std::size_t last_pos = 0;
    auto pos = data.find(separator, last_pos);
    while(pos != std::string::npos)
    {
        ret.emplace_back(string_to<T>(data.substr(last_pos, pos-last_pos)));
        last_pos = pos + separator.size();
        pos = data.find(separator, last_pos);
    }
    // Handle not having separator at end of a line
    if(last_pos < data.size())
    {
        ret.emplace_back(string_to<T>(data.substr(last_pos)));
    }
    return ret;
};

template<class T>
std::vector<T> file_to_vec_sep(std::string const& filename, std::string const & sub_sep, std::string const& separator=NEW_LINE)
{
    auto data = file_to_string(filename);
    return str_to_vec_sep<T>(data, sub_sep, separator);
}

template<class T>
std::vector<T> file_to_vec(std::string const& filename, std::string const& separator=NEW_LINE)
{
    auto data = file_to_string(filename);
    return str_to_vec<T>(data, separator);
}

template<class T>
void print_vec(std::vector<T> const& data)
{
    for(auto &&str : data)
    {
        std::cout << str << "\n";
    }
}

template<class T>
std::vector<std::vector<T>> str_to_2d(std::string const& data, std::string const & sub_sep, std::string const& separator=NEW_LINE)
{
    auto lines = str_to_vec<std::string>(data, separator);
    auto ret = std::vector<std::vector<T>>{};
    for(auto &&l : lines)
    {
        ret.emplace_back(str_to_vec<T>(l, sub_sep));
    }
    return ret;
};

template<class T>
std::vector<std::vector<T>> file_to_2d(std::string const& filename, std::string const & sub_sep, std::string const& separator=NEW_LINE)
{
    auto data = file_to_string(filename);
    return str_to_2d<T>(data, sub_sep, separator);
}

template<class T>
void print_2d(std::vector<std::vector<T>> const& data, std::size_t width=3, char pad=' ')
{
    for(auto &&x : data)
    {
        for(auto &&y : x)
        {
            std::cout << std::setfill(pad) << std::setw(width) << y << ", ";
        }
        std::cout << "\n";
    }
}

struct area {
    std::size_t row_start;
    std::size_t col_start;
    std::size_t row_end;
    std::size_t col_end;

    std::size_t row_size()
    {
        return row_end - row_start + 1;
    };
    std::size_t col_size()
    {
        return col_end - col_start + 1;
    };
    std::size_t area_size()
    {
        return row_size() * col_size();
    };
};

template<class T>
area get_bounding_box(std::vector<T> const & input_vec, area const & input_area, std::size_t width)
{
    auto ret = area{0,0,0,0};
    auto col_size = input_vec[0].size();
    auto row_size = input_vec.size();

    ret.col_start = input_area.col_start >= width ? input_area.col_start - width : 0;
    ret.col_end = input_area.col_end <= col_size - 1 - width ? input_area.col_end + width : col_size -1;
    ret.row_start = input_area.row_start >= width ? input_area.row_start - width : 0;
    ret.row_end = input_area.row_end <= row_size - 1 - width ? input_area.row_end + width : row_size -1;

    return ret;
};