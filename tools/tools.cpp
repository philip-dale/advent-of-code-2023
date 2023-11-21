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