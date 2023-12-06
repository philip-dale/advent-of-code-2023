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


void dijkstra(std::vector<node> & nodes, std::size_t start, bool do_all, std::size_t end)
{
    auto current_node = start;
    nodes[current_node].update_dist(0);
    bool running = true;
    while(running)
    {
        nodes[current_node].set_previous(0);
        
        for(auto neighbour : nodes[current_node].neighbours())
        {
            if(!nodes[neighbour].processed())
            {
                auto new_dist = nodes[current_node].dist() + nodes[neighbour].weight();
                nodes[neighbour].update_dist(new_dist);
            }
        }
        if(!do_all && current_node == end)
        {
            return;
        }

        auto next_dist = std::numeric_limits<std::uint32_t>::max();
        running = false;
        for(auto &&node : nodes)
        {
            if(!node.processed() && node.dist() < next_dist)
            {
                current_node = node.id();
                next_dist = node.dist();
                running = true;
            }
        }
    }
}