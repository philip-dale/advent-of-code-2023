#include "../tools/tools.h"
#include "../tools/types.h"

#include <cstdint>
#include <iostream>

bool sum_zero(std::vector<std::int64_t> & v)
{
    for(auto &&n : v)
    {
        if(n != 0)
        {
            return false;
        }
    }
    return true;
}

std::vector<std::int64_t> get_diff(std::vector<std::int64_t> & v)
{
    auto ret = std::vector<std::int64_t>{};
    for(auto i=1; i< v.size(); ++i)
    {
        ret.emplace_back(v[i] - v[i-1]);
    }
    return ret;
}

std::pair<std::int64_t,std::int64_t> game(std::string file_name)
{
    auto lines = file_to_vec<std::string>(file_name);
    auto sum_next = std::int64_t{0};
    auto sum_prev = std::int64_t{0};
    for(auto &&l : lines)
    {
        auto patterns = std::vector<std::vector<std::int64_t>>{};
        patterns.emplace_back(str_to_vec<std::int64_t>(l, " "));

        while(!sum_zero(patterns[patterns.size()-1]))
        {
            patterns.emplace_back(get_diff(patterns[patterns.size()-1]));
        }

        auto next_diff = std::int64_t{0};
        auto prev_diff = std::int64_t{0};
        for(std::int64_t i=patterns.size()-2; i >=0; --i)
        {
            next_diff += patterns[i][patterns[i].size()-1];
            prev_diff = patterns[i][0] - prev_diff;
        }
        sum_next += next_diff;
        sum_prev += prev_diff;
    }
    return {sum_prev, sum_next};
}


void part1()
{
    std::cout << game("input_actual").second << "\n";
}

void part2()
{
    std::cout << game("input_actual").first << "\n";
}

int main(int argc, char* argv[])
{
    std::cout << "---- Part1 ----\n";
    part1();
    std::cout << "---- Part2 ----\n";
    part2();
}