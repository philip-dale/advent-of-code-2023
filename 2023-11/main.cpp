#include "../tools/tools.h"
#include "../tools/types.h"

#include <cstdint>
#include <iostream>
#include <cmath>

void part1()
{
    auto lines = file_to_vec<std::string>("input_actual");
    auto col_sums = std::vector<std::uint32_t>(lines[0].size(), 0);
    auto row_sums = std::vector<std::uint32_t>{};
    auto universe = std::vector<std::vector<char>>{};

    for(auto &&l : lines)
    {
        auto row = std::vector<char>{};
        row_sums.emplace_back(0);
        for(auto i=0; i<l.size(); ++i)
        {
            row.emplace_back(l[i]);
            if(l[i] == '#')
            {
                col_sums[i]++;
                row_sums[row_sums.size()-1]++;
            }
        }
        universe.emplace_back(row);
    }
    
    auto galaxies = std::vector<std::pair<std::int64_t, std::int64_t>>{};
    for(auto r=0; r<universe.size(); ++r)
    {
        for(auto c=0; c<universe[0].size(); ++c)
        {
            if(universe[r][c] == '#')
            {
                galaxies.emplace_back(std::pair<std::int64_t, std::int64_t>{r,c});
            }
        }
    }

    auto increment = 1;
    // expand galaxies
    for(auto g=0; g<galaxies.size(); ++g)
    {
        auto g_pos_row = galaxies[g].first;
        for(auto r=0; r<g_pos_row; ++r)
        {
            if(row_sums[r] == 0)
            {
                galaxies[g].first += increment;
            }
        }

        auto g_pos_col = galaxies[g].second;
        for(auto c=0; c<g_pos_col; ++c)
        {
            if(col_sums[c] == 0)
            {
                galaxies[g].second += increment;
            }
        }
    }

    auto distance_sum = std::uint64_t{0};
    for(auto g=0; g<galaxies.size()-1; ++g)
    {
        for( auto g_iter = galaxies.begin()+g+1; g_iter<galaxies.end(); ++g_iter)
        {
            auto distance = std::abs(galaxies[g].first - g_iter->first) + std::abs(galaxies[g].second - g_iter->second);
            distance_sum += distance;
        }
    }

    std::cout << distance_sum << "\n";
}

void part2()
{
    auto lines = file_to_vec<std::string>("input_actual");
    auto col_sums = std::vector<std::uint32_t>(lines[0].size(), 0);
    auto row_sums = std::vector<std::uint32_t>{};
    auto universe = std::vector<std::vector<char>>{};

    for(auto &&l : lines)
    {
        auto row = std::vector<char>{};
        row_sums.emplace_back(0);
        for(auto i=0; i<l.size(); ++i)
        {
            row.emplace_back(l[i]);
            if(l[i] == '#')
            {
                col_sums[i]++;
                row_sums[row_sums.size()-1]++;
            }
        }
        universe.emplace_back(row);
    }
    
    auto galaxies = std::vector<std::pair<std::int64_t, std::int64_t>>{};
    for(auto r=0; r<universe.size(); ++r)
    {
        for(auto c=0; c<universe[0].size(); ++c)
        {
            if(universe[r][c] == '#')
            {
                galaxies.emplace_back(std::pair<std::int64_t, std::int64_t>{r,c});
            }
        }
    }

    auto increment = 1000000-1;
    // expand galaxies
    for(auto g=0; g<galaxies.size(); ++g)
    {
        auto g_pos_row = galaxies[g].first;
        for(auto r=0; r<g_pos_row; ++r)
        {
            if(row_sums[r] == 0)
            {
                galaxies[g].first += increment;
            }
        }

        auto g_pos_col = galaxies[g].second;
        for(auto c=0; c<g_pos_col; ++c)
        {
            if(col_sums[c] == 0)
            {
                galaxies[g].second += increment;
            }
        }
    }

    auto distance_sum = std::uint64_t{0};
    for(auto g=0; g<galaxies.size()-1; ++g)
    {
        for( auto g_iter = galaxies.begin()+g+1; g_iter<galaxies.end(); ++g_iter)
        {
            auto distance = std::abs(galaxies[g].first - g_iter->first) + std::abs(galaxies[g].second - g_iter->second);
            distance_sum += distance;
        }
    }

    std::cout << distance_sum << "\n";
}

int main(int argc, char* argv[])
{
    std::cout << "---- Part1 ----\n";
    part1();
    std::cout << "---- Part2 ----\n";
    part2();
}