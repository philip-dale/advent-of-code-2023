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

typedef std::pair<std::int64_t,std::int64_t> game_t;

game_t game(std::string file_name)
{
    auto ret = game_t{0,0};
    auto lines = file_to_vec<std::string>(file_name);
    for(auto &&l : lines)
    {
        auto patterns = std::vector<std::vector<std::int64_t>> {
            str_to_vec<std::int64_t>(l, " ")
        };

        while(!sum_zero(patterns[patterns.size()-1]))
        {
            patterns.emplace_back(get_diff(patterns[patterns.size()-1]));
        }
        game_t diff{0,0};
        for(std::int64_t i=patterns.size()-2; i >=0; --i)
        {
            diff.second += patterns[i][patterns[i].size()-1];
            diff.first = patterns[i][0] - diff.first;
        }
        ret.second += diff.second;
        ret.first += diff.first;
    }
    return ret;
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