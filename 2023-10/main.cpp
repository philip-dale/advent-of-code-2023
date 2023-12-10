#include "../tools/tools.h"
#include "../tools/types.h"

#include <cstdint>
#include <iostream>
#include <map>

enum class direction {
    left = 1,
    right,
    up,
    down,
};

bool directions_contains(std::vector<direction> const & v, direction d)
{
    return std::find(v.begin(), v.end(), d) != v.end();
}

std::map<char, std::vector<direction>> symbols{
    {'|', {direction::up, direction::down}},
    {'-', {direction::left, direction::right}},
    {'L', {direction::up, direction::right}},
    {'J', {direction::up, direction::left}},
    {'7', {direction::left, direction::down}},
    {'F', {direction::right, direction::down}},
    {'.', {}},
    {'I', {}},
    {'O', {}},
    {'S', {direction::up, direction::down,direction::left, direction::right}}
};

std::vector<std::pair<std::size_t, std::size_t>> get_neighbours(std::vector<std::vector<char>> & data, std::size_t x, std::size_t y)
{
    auto neighbours = std::vector<std::pair<std::size_t, std::size_t>>{};
    if(x != 0){
        if(directions_contains(symbols[data[y][x-1]], direction::right) && directions_contains(symbols[data[y][x]], direction::left))
        {
            neighbours.emplace_back(std::pair<std::size_t, std::size_t>(x-1, y));
        }
    }
    if(x != data[0].size()-1)
    {   
        if(directions_contains(symbols[data[y][x+1]], direction::left) && directions_contains(symbols[data[y][x]], direction::right))
        {
            neighbours.emplace_back(std::pair<std::size_t, std::size_t>(x+1, y));
        }
    }
    if(y != 0)
    {
        if(directions_contains(symbols[data[y-1][x]], direction::down) && directions_contains(symbols[data[y][x]], direction::up))
        {
            neighbours.emplace_back(std::pair<std::size_t, std::size_t>(x, y-1));
        }
    }
    if(y != data.size()-1)
    {
        if(directions_contains(symbols[data[y+1][x]], direction::up) && directions_contains(symbols[data[y][x]], direction::down))
        {
            neighbours.emplace_back(std::pair<std::size_t, std::size_t>(x, y+1));
        }
    }
    return neighbours;
}

void part1()
{
    auto map_lines = file_to_vec<std::string>("input_actual");
    auto data = std::vector<std::vector<char>>{};
    for(auto && l : map_lines)
    {
        auto v = std::vector<char>{};
        for(auto && c : l)
        {
            v.emplace_back(c);
        }
        data.emplace_back(v);
    }

    auto start_node = std::pair<std::size_t, std::size_t>{0,0};
    for(auto i=0; i<data.size(); ++i)
    {
        for(auto j=0; j<data.size(); ++j)
        {
            if(data[i][j] == 'S')
            {
                start_node = std::pair<std::size_t, std::size_t>{j,i};
            }
        }
    }

    auto previous_node = start_node;
    auto current_node = start_node;
    auto step_count = std::size_t{0};
    do
    {
        auto neighbours = get_neighbours(data, current_node.first, current_node.second);
        for(auto &&n : neighbours)
        {
            if(!(n.first == previous_node.first && n.second == previous_node.second))
            {
                previous_node = current_node;
                current_node.first = n.first;
                current_node.second = n.second;
                break;
            }
        }
        step_count++;

    } 
    while (data[current_node.second][current_node.first] != 'S');
    
    std::cout << step_count/2 << "\n";
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

void part2()
{
    auto map_lines = file_to_vec<std::string>("input_actual");
    auto data = std::vector<std::vector<char>>{};
    for(auto && l : map_lines)
    {
        auto v = std::vector<char>{};
        for(auto && c : l)
        {
            v.emplace_back(c);
        }
        data.emplace_back(v);
        
    }

    auto start_node = std::pair<std::size_t, std::size_t>{0,0};
    for(auto i=0; i<data.size(); ++i)
    {
        for(auto j=0; j<data.size(); ++j)
        {
            if(data[i][j] == 'S')
            {
                start_node = std::pair<std::size_t, std::size_t>{j,i};
            }
        }
    }
    auto S_neighbours = get_neighbours(data, start_node.first, start_node.second);
    bool S_is_vertex = true;
    if(S_neighbours[0].first == S_neighbours[1].first || S_neighbours[0].second == S_neighbours[1].second)
    {
        S_is_vertex = false;
    }  

    auto previous_node = start_node;
    auto current_node = start_node;
    auto shape = std::vector<std::pair<std::size_t, std::size_t>>{};
    
    do
    {
        shape.emplace_back(current_node);
        auto neighbours = get_neighbours(data, current_node.first, current_node.second);
        for(auto &&n : neighbours)
        {
            if(!(n.first == previous_node.first && n.second == previous_node.second))
            {
                previous_node = current_node;
                current_node.first = n.first;
                current_node.second = n.second;
                break;
            }
        }
    } 
    while (data[current_node.second][current_node.first] != 'S');

    auto shape_vertex = std::vector<std::pair<std::int64_t, std::int64_t>>{};
    for(auto &&p : shape)
    {
        char v = data[p.second][p.first];
        if( v == 'F' || v == 'J' || v == 'L' || v == '7' || (v == 'S' && S_is_vertex))
        {
            auto vertex = std::pair<std::int64_t, std::int64_t>{static_cast<int64_t>(p.first), static_cast<int64_t>(p.second)};
            shape_vertex.emplace_back(vertex);
        }
    }

    auto area = get_area(shape_vertex);
    // Pick Theorem states that: S = I + B / 2 - 1 Where S — polygon area, I — number of points strictly inside polygon and B — Number of points on boundary.
    auto pick = area - (shape.size()/2) + 1;

    std::cout << pick << "\n";
}

int main(int argc, char* argv[])
{
    std::cout << "---- Part1 ----\n";
    part1();
    std::cout << "---- Part2 ----\n";
    part2();
}