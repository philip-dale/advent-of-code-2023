#include "../tools/tools.h"
#include "../tools/types.h"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

std::vector<std::uint64_t> get_totals(std::vector<std::string> input)
{
    std::vector<std::uint64_t> totals{};
    std::uint64_t total = 0;
    for(auto &&l : input)
    {
        if(l.empty())
        {
            totals.emplace_back(total);
            total = 0;
        }
        else
        {
            total += stol(l);
        }
    }
    std::sort(totals.begin(), totals.end());
    return totals;
}

void part1()
{
    auto input = file_to_vec<std::string>("actual.txt");
    auto totals = get_totals(input);
    auto size = totals.size();
    std::cout << totals[size-1] << "\n";
}

void part2()
{
    auto input = file_to_vec<std::string>("actual.txt");
    auto totals = get_totals(input);
    auto size = totals.size();
    std::cout << totals[size-1] + totals[size-2] + totals[size-3] << "\n";
}

int main(int argc, char* argv[])
{
    std::cout << "---- Part1 ----\n";
    part1();
    std::cout << "---- Part2 ----\n";
    part2();
}