#include "../tools/tools.h"
#include "../tools/types.h"

#include <cstdint>
#include <iostream>
#include <cmath>
#include <map>
#include <algorithm>

struct ToDo{
    std::int64_t dist;
    std::int64_t r;
    std::int64_t c;
    std::int64_t dr;
    std::int64_t dc;
    std::int64_t steps;
    std::string key()
    {
        return std::string(std::to_string(r) + "_" + std::to_string(c) + "_" + std::to_string(dr) + "_" + std::to_string(dc) + "_" + std::to_string(steps));
    }
};

void walk(std::vector<std::vector<std::uint32_t>> & weights, std::vector<std::vector<std::uint32_t>> & dists, std::int64_t min_range, std::int64_t max_range)
{
    std::map<std::string, std::uint32_t> cache{};
    auto stack = std::vector<ToDo>{{0,0,0,0,0,0}};

    while(stack.size() > 0)
    {
        // Like Dijkstra lets get the lowest value distance to do next
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

        auto next_test = std::move(stack[low_pos]);
        stack.erase(stack.begin() + low_pos);

        // update our map if we have a better score
        if(next_test.dist < dists[next_test.r][next_test.c])
        {
            dists[next_test.r][next_test.c] = next_test.dist;
        }

        if(next_test.r == weights.size()-1 && next_test.c == weights[0].size()-1)
        {
            return;
        }

        // dont repeat my self
        if(cache.contains(next_test.key()))
        {
            continue;
        }
        cache[next_test.key()] = next_test.dist;

        // test in straight line, catch initial state
        if(next_test.steps < max_range && !(next_test.dr == 0 && next_test.dc == 0))
        {
            auto r_next = next_test.r + next_test.dr;
            auto c_next = next_test.c + next_test.dc;
            if(r_next >= 0 && r_next < weights.size() && c_next >= 0 && c_next < weights[0].size())
            {
                stack.emplace_back(ToDo{next_test.dist + weights[r_next][c_next], r_next, c_next, next_test.dr, next_test.dc, next_test.steps+1});
            }
        }

        // Test turning, catch initial state
        if(next_test.steps >= min_range || (next_test.dr == 0 && next_test.dc == 0))
        {
            // because de cannot move on diagonal, either dr or dc must be 0
            if(next_test.dc != 0)
            {
                if(next_test.r+1 < weights.size())
                {
                    stack.emplace_back(ToDo{next_test.dist + weights[next_test.r+1][next_test.c], next_test.r+1, next_test.c, 1, 0, 1});
                }
                if(next_test.r-1 >= 0)
                {
                    stack.emplace_back(ToDo{next_test.dist + weights[next_test.r-1][next_test.c], next_test.r-1, next_test.c, -1, 0, 1});
                }
            }
            else
            {   
                if(next_test.c+1 < weights[0].size())
                {
                    stack.emplace_back(ToDo{next_test.dist + weights[next_test.r][next_test.c+1], next_test.r, next_test.c+1, 0, 1, 1});
                }
                if(next_test.c-1 >= 0)
                {
                    stack.emplace_back(ToDo{next_test.dist + weights[next_test.r][next_test.c-1], next_test.r, next_test.c-1, 0, -1, 1});
                }
            }
        }
    }
}

void run(std::string const& file_name, std::int64_t min_range, std::int64_t max_range)
{
    auto map_lines = file_to_vec<std::string>(file_name);
    auto weights = std::vector<std::vector<std::uint32_t>>{};
    auto dists = std::vector<std::vector<std::uint32_t>>{};
    for(auto && l : map_lines)
    {
        auto v = std::vector<std::uint32_t>{};
        auto d = std::vector<std::uint32_t>{};
        for(auto && c : l)
        {
            v.emplace_back(c - 0x30);
            d.emplace_back(std::numeric_limits<std::uint32_t>::max());
        }
        weights.emplace_back(v);
        dists.emplace_back(d);
    }
    walk(weights, dists, min_range, max_range);
    std::cout << dists[dists.size()-1][dists[0].size()-1] << "\n";
}

void part1()
{
    run("input_actual", 0, 3);
}

void part2()
{
    run("input_actual", 4, 10);
}

int main(int argc, char* argv[])
{
    std::cout << "---- Part1 ----\n";
    part1();
    std::cout << "---- Part2 ----\n";
    part2();
}