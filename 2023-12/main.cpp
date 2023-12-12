#include "../tools/tools.h"
#include "../tools/types.h"

#include <cstdint>
#include <iostream>

std::uint32_t count_possibles(std::string & group, std::size_t group_start, std::vector<std::size_t> & counts, std::size_t counts_start)
{

    if(group.size() == group_start)
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

    auto count = std::uint32_t{0};

    // process . or if ? is a .
    if(group[group_start] == '.' || group[group_start] == '?')
    {
        count += count_possibles(group, group_start+1, counts, counts_start);
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

        if(posible_fail_size < counts[counts_start])
        {
            // wont fit
            return count;
        }
        // Character after match cannot be a #
        if(group_start + counts[counts_start] < group.size())
        {
            if(group[group_start + counts[counts_start]] == '#')
            {
                return count;
            }
        }
        count += count_possibles(group, group_start+counts[counts_start]+1, counts, counts_start+1);
    }
    

    return count;
}

void part1()
{
    auto lines = file_to_vec<std::string>("input_actual");
    auto sum = std::uint32_t{0};
    for(auto &&l : lines)
    {
        auto parts = str_to_vec<std::string>(l, " ");
        auto group = parts[0];
        auto counts = str_to_vec<std::size_t>(parts[1], ",");
        auto line_count = count_possibles(group, 0, counts, 0);
        // std::cout << "Line = " << line_count << "\n";
        sum += line_count;
    }
    std::cout << sum << "\n";
}

void part2()
{
}

int main(int argc, char* argv[])
{
    std::cout << "---- Part1 ----\n";
    part1();
    std::cout << "---- Part2 ----\n";
    part2();
}