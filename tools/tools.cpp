#include "tools.h"

#include <fstream>
#include <iostream>
#include <streambuf>
#include <cerrno>

std::string to_string(std::vector<std::uint8_t> const & data)
{
    return std::string(data.begin(), data.end());
}

std::string to_hex_string(std::vector<std::uint8_t> const & data)
{
    const std::string chars = "0123456789ABCDEF";
    std::string ret{};
    for(auto &&val : data)
    {
        ret.push_back(chars[(val & 0xF0) >> 4]);
        ret.push_back(chars[(val & 0x0F)]);
    }
    return ret;
}

std::string file_to_string(std::string const& filename)
{
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if(in)
    {
        return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    }
    throw(errno);
}

std::vector<double> solve_quad(double a, double b, double c)
{
    auto discriminant = (b*b) - (4*a*c);
    auto result = std::vector<double>{};
    if (discriminant > 0) {
        result.emplace_back((-b + std::sqrt(discriminant)) / (2*a));
        result.emplace_back((-b - std::sqrt(discriminant)) / (2*a));
    }
    else if (discriminant == 0) {
        result.emplace_back(-b/(2*a));
        result.emplace_back(-b/(2*a));
    }
    else {
        // not doing imaginary numbers
    }
    return result;
}

std::string remove_space(std::string & in)
{
    auto out = std::string{};
    for(auto &&c : in)
    {
        if(c != ' ')
        {
            out += c;
        }
    }
    return out;
}