#include "../tools/tools.h"
#include "../tools/types.h"

#include <cstdint>
#include <iostream>
#include <map>

typedef std::map<std::string, std::uint64_t> cache_map;

std::uint64_t count_possibles(std::string const& group, std::size_t const group_start, std::vector<std::size_t> const & counts, std::size_t const counts_start, cache_map & cache)
{
    if(group_start >= group.size())
    {
        if(counts.size() == counts_start)
        {
            return 1;
        }
        return 0;
    }
    else
    {
        if(counts.size() == counts_start)
        {
            if(group.find_first_of('#', group_start) == std::string::npos)
            {
                return 1;
            }
            return 0;
        }
    }

    auto cache_key = std::to_string(group_start) + "_" + std::to_string(counts_start);
    if(cache.contains(cache_key))
    {
        return cache[cache_key];
    }

    auto count = std::uint64_t{0};

    // process . or if ? is a .
    if(group[group_start] == '.' || group[group_start] == '?')
    {
        count += count_possibles(group, group_start+1, counts, counts_start, cache);
    }

    // process # or if ? is a #
    if(group[group_start] == '#' || group[group_start] == '?')
    {
        auto posible_fail_end = group.find_first_of('.', group_start);
        if(posible_fail_end == std::string::npos)
        {
            posible_fail_end = group.size();
        }
        auto posible_fail_size = posible_fail_end - group_start;

        // Check we will fit
        if(posible_fail_size >= counts[counts_start])
        {
            // Character after match cannot be a #
            if(!(group_start + counts[counts_start] < group.size() && group[group_start + counts[counts_start]] == '#'))
            {
                count += count_possibles(group, group_start+counts[counts_start]+1, counts, counts_start+1, cache);
            }
        }
    }

    cache[cache_key] = count;
    return count;
}

void part1()
{
    auto lines = file_to_vec<std::string>("input_actual");
    auto sum = std::uint64_t{0};
    for(auto &&l : lines)
    {
        auto parts = str_to_vec<std::string>(l, " ");
        auto group = parts[0];
        auto counts = str_to_vec<std::size_t>(parts[1], ",");
        auto cache = cache_map{};
        sum += count_possibles(group, 0, counts, 0, cache);
    }
    std::cout << sum << "\n";
}

void part2()
{
    auto lines = file_to_vec<std::string>("input_actual");
    auto sum = std::uint64_t{0};
    for(auto &&l : lines)
    {
        auto parts = str_to_vec<std::string>(l, " ");

        auto group_base = parts[0];
        auto group = group_base;
        auto counts_base = str_to_vec<std::size_t>(parts[1], ",");
        auto counts = counts_base;
        for(auto i=0; i<4; i++)
        {
            group += "?"+group_base;
            for(auto &&c : counts_base)
            {
                counts.emplace_back(c);
            }
        }
        auto cache = cache_map{};
        sum += count_possibles(group, 0, counts, 0, cache);
    }
    std::cout << sum << "\n";
}

int main(int argc, char* argv[])
{
    std::cout << "---- Part1 ----\n";
    part1();
    std::cout << "---- Part2 ----\n";
    part2();
}