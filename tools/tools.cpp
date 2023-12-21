#include "tools.h"

#include <fstream>
#include <iostream>
#include <streambuf>
#include <cerrno>
#include <map>

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

struct dijkstra_node
{
    std::int64_t dist;
    std::int64_t r;
    std::int64_t c;
    std::string key()
    {
        return std::string(std::to_string(r) + "_" + std::to_string(c));
    }
};

void dijkstra_2d(std::vector<std::vector<std::uint32_t>> & weights, std::vector<std::vector<std::uint32_t>> & dists)
{
    std::map<std::string, std::uint32_t> cache{};
    auto stack = std::vector<dijkstra_node>{{0,0,0}};
    while(stack.size() > 0)
    {
        auto low_val = stack[0].dist;
        auto low_pos = 0;
        for(auto i=0; i< stack.size(); ++i)
        {
            if(stack[i].dist < low_val)
            {
                low_val = stack[i].dist;
                low_pos = i;
            }
        }

        auto next_test = stack[low_pos];
        stack.erase(stack.begin() + low_pos);

        // update our map if we have a better score
        if(next_test.dist < dists[next_test.r][next_test.c])
        {
            dists[next_test.r][next_test.c] = next_test.dist;
        }

        // dont repeat my self
        if(cache.contains(next_test.key()))
        {
            continue;
        }
        cache[next_test.key()] = next_test.dist;

        for(auto neighbour : adjacent_neighbours_2d(weights, next_test.c, next_test.r))
        {
            stack.emplace_back(dijkstra_node{next_test.dist + weights[neighbour.second][neighbour.first], static_cast<std::uint32_t>(neighbour.second), static_cast<std::uint32_t>(neighbour.first)});
        }
    }
}

// Calculate value of shoelace formula
std::int64_t get_area(std::vector<std::pair<std::int64_t, std::int64_t>> &shape)
{
    auto area = std::int64_t{0};
    auto j = shape.size() - 1;
    for (auto i = 0; i < shape.size(); ++i)
    {
        area += (shape[j].first - shape[i].first) * (shape[j].second + shape[i].second);
        j = i;
    }
    return std::abs(area) / 2;
}

std::int64_t get_pick_area(std::vector<std::pair<std::int64_t, std::int64_t>> &shape, std::size_t edge_count)
{
    auto area = get_area(shape);
    return area - (edge_count/2) + 1;
}

//use  Lagrange Interpolation
double solve_poly_quad(std::vector<double> & x, std::vector<double> & y, double pos)
{
    double result = 0;
    for (int i=0; i<x.size(); i++)
    {
        double term = y[i];
        for (int j=0;j<x.size();j++)
        {
            if (j!=i)
            {
                term = term*(pos - x[j])/(x[i] - x[j]);
            }
        }
        result += term;
    }
    return result;
}