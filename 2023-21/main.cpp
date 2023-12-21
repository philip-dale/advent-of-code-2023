#include "../tools/tools.h"
#include "../tools/types.h"

#include <cstdint>
#include <iostream>
#include <map>

struct dijkstra_garden_node
{
    std::int64_t dist;
    std::int64_t r;
    std::int64_t c;
    std::string key()
    {
        return std::string(std::to_string(r) + "_" + std::to_string(c));
    }
};

std::vector<std::pair<std::int64_t, std::int64_t>> adjacent_neighbours_infinate(std::int64_t x, std::int64_t y)
{
    auto neighbours = std::vector<std::pair<std::int64_t, std::int64_t>>{};
    neighbours.emplace_back(std::pair<std::int64_t, std::int64_t>(x-1, y));
    neighbours.emplace_back(std::pair<std::int64_t, std::int64_t>(x+1, y));
    neighbours.emplace_back(std::pair<std::int64_t, std::int64_t>(x, y-1));
    neighbours.emplace_back(std::pair<std::int64_t, std::int64_t>(x, y+1));  
    return neighbours;
}

char get_at_pos(std::vector<std::vector<char>> & garden, std::int64_t x, std::int64_t y)
{
    while(x < 0)
    {
        // x = garden[0].size() + x;
        x = garden.size() - ((x*-1) % garden[0].size());
    }
    while(x > garden[0].size()-1)
    {
        x = x % garden[0].size();
    }

    while(y < 0)
    {
        // y = garden.size() + y;
        y = garden.size() - ((y*-1) % garden.size());
    }
    while(y > garden.size()-1)
    {
        y = y % garden.size();
    }
    return garden[x][y];
}

std::uint64_t dijkstra_garden(std::vector<std::vector<char>> & garden, std::uint64_t max_dist, bool infinite)
{
    std::map<std::string, std::int64_t> cache{};
    auto dist_count = std::uint64_t{0};
    auto is_even = max_dist % 2 == 0;

    auto stack = std::vector<dijkstra_garden_node>{};
    for(auto r=0; r<garden.size(); ++r)
    {
        for(auto c=0; c<garden[0].size(); ++c)
        {
            if(garden[r][c] == 'S')
            {
                stack.emplace_back(dijkstra_garden_node{0,r,c});
            }
        }
    }

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

        // dont repeat my self
        if(cache.contains(next_test.key()))
        {
            continue;
        }
        cache[next_test.key()] = next_test.dist;

        if(next_test.dist <= max_dist)
        {
            if((is_even && next_test.dist % 2 == 0) || (!is_even && next_test.dist % 2 != 0))
            {
                dist_count++;
            }
        }

        if(next_test.dist > max_dist)
        {
            continue;
        }

        if(!infinite)
        {
            for(auto neighbour : adjacent_neighbours_2d(garden, next_test.c, next_test.r))
            {
                if(garden[neighbour.second][neighbour.first] == '#')
                {
                    continue;
                }
                // Step size is always 1
                stack.emplace_back(dijkstra_garden_node{next_test.dist + 1, static_cast<std::int64_t>(neighbour.second), static_cast<std::int64_t>(neighbour.first)});
            }
        }
        else
        {
            for(auto neighbour : adjacent_neighbours_infinate(next_test.c, next_test.r))
            {
                if(get_at_pos(garden, neighbour.first, neighbour.second) == '#')
                {
                    continue;
                }
                // Step size is always 1
                stack.emplace_back(dijkstra_garden_node{next_test.dist + 1, static_cast<std::int64_t>(neighbour.second), static_cast<std::int64_t>(neighbour.first)});
            }
        }
    }
    return dist_count;
}

void part1()
{
    auto map_lines = file_to_vec<std::string>("input_actual");
    auto garden = std::vector<std::vector<char>>{};
    for(auto && l : map_lines)
    {
        auto v = std::vector<char>{};
        for(auto && c : l)
        {
            v.emplace_back(c);
        }
        garden.emplace_back(v);
    }
    auto result = dijkstra_garden(garden, 64, false);

    std::cout << result << "\n";
}

void part2()
{
    auto map_lines = file_to_vec<std::string>("input_sample");
    auto garden = std::vector<std::vector<char>>{};
    for(auto && l : map_lines)
    {
        auto v = std::vector<char>{};
        for(auto && c : l)
        {
            v.emplace_back(c);
        }
        garden.emplace_back(v);
    }
    auto result = dijkstra_garden(garden, 5000, true);

    std::cout << result << "\n";
}

int main(int argc, char* argv[])
{
    std::cout << "---- Part1 ----\n";
    part1();
    std::cout << "---- Part2 ----\n";
    part2();
}