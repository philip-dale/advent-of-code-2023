#include "../tools/tools.h"
#include "../tools/types.h"

#include <cstdint>
#include <iostream>
#include <regex>

typedef std::pair<std::int64_t, std::int64_t> hole_point;

std::int64_t hex_to_int(std::string const & hex)
{
    std::int64_t v;   
    std::stringstream ss;
    ss << std::hex << hex;
    ss >> v;
    return v;
}

void part1()
{
    auto lines = file_to_vec<std::string>("input_actual");
    auto points = std::vector<hole_point>{};
    auto edge_count = std::int64_t{0};
    auto current = hole_point{0,0};
    for(auto &&l : lines)
    {
        points.emplace_back(current);
        auto parts = std::regex("([UDLR]) (.+) (.+)");
        std::smatch matches;
        std::regex_search(l, matches, parts);
        auto dist = stoi(matches[2].str());
        char dir = matches[1].str()[0];
        switch(dir)
        {
            case 'U':
                current.first += dist;
                break;
            case 'D':
                current.first -= dist;
                break;
            case 'L':
                current.second += dist;
                break;
            case 'R':
                current.second -= dist;
                break;
        }
        edge_count +=dist;
    }
    std::cout << get_pick_area(points, edge_count) + edge_count  << "\n";
}

void part2()
{
    auto lines = file_to_vec<std::string>("input_actual");
    auto points = std::vector<hole_point>{};
    auto edge_count = std::int64_t{0};
    auto current = hole_point{0,0};
    for(auto &&l : lines)
    {
        points.emplace_back(current);
        auto parts = std::regex("([UDLR]) (.+) \\(#(.{5})(.)\\)");
        std::smatch matches;
        std::regex_search(l, matches, parts);
        auto dist = hex_to_int(matches[3].str());
        char dir = matches[4].str()[0];
        switch(dir)
        {
            case '3':
                current.first += dist;
                break;
            case '1':
                current.first -= dist;
                break;
            case '2':
                current.second += dist;
                break;
            case '0':
                current.second -= dist;
                break;
        }
        edge_count +=dist;
    }
    std::cout << get_pick_area(points, edge_count) + edge_count  << "\n";
}

int main(int argc, char* argv[])
{
    std::cout << "---- Part1 ----\n";
    part1();
    std::cout << "---- Part2 ----\n";
    part2();
}