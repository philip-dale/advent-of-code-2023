#include "../tools/tools.h"
#include "../tools/types.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>

bool scan_number(std::vector<std::string> const & input, std::size_t num_start, std::size_t num_size, std::size_t num_row)
{
    auto box = get_bounding_box<std::string>(input, area{num_row, num_start, num_row, num_start + num_size-1}, 1);
 
    for(auto col = box.col_start; col <= box.col_end; ++col) 
    {
        for(auto row = box.row_start; row <= box.row_end; ++row) 
        {
            if(row == num_row && ( col > box.col_start && col < box.col_end) )
            {
                continue;
            }
            char current_char = input[row][col];
            if(!isdigit(current_char) && current_char != '.')
            {
                return true;
            }
        }
    }

    return false;
}

bool has_gear(std::vector<std::string> const & input, std::size_t num_start, std::size_t num_size, std::size_t num_row, std::size_t & g_col, std::size_t & g_row)
{
    auto box = get_bounding_box<std::string>(input, area{num_row, num_start, num_row, num_start + num_size-1}, 1);
    
    for(g_col = box.col_start; g_col <= box.col_end; ++g_col) 
    {
        for(g_row = box.row_start; g_row <= box.row_end; ++g_row) 
        {
            if(g_row == num_row && ( g_col > box.col_start && g_col < box.col_end ) )
            {
                continue;
            }
            char current_char = input[g_row][g_col];
            if(current_char == '*')
            {
                return true;
            }
        }
    }
    return false;
}

void part1()
{
    // auto input = file_to_vec<std::string>("input_sample");
    auto input = file_to_vec<std::string>("input_actual");

    auto col_size = input[0].size();
    auto row_size = input.size();

    std::uint32_t sum{0};

    for(auto row = 0; row < row_size; ++row)
    {
        for(auto col = 0; col < col_size; ++col)
        {
            if(!isdigit(input[row][col]))
            {
                continue;
            }

            // Find size of number
            auto num_end = col;
            while( num_end< col_size && isdigit(input[row][num_end]))
            {
                num_end++;
            }
            auto num_size = num_end - col;

            // Look for symbols
            if(scan_number(input, col, num_size, row))
            {
                sum += std::stoi(input[row].substr(col, num_size));
            }
            col += num_size;
        }
    }
    std::cout << sum << "\n";
}

struct num_gear{
    std::int32_t num;
    std::size_t g_col;
    std::size_t g_row;

    bool operator==(const num_gear& b)
    {
        return g_col == b.g_col && g_row == b.g_row;
    }
};


void part2()
{
    // auto input = file_to_vec<std::string>("input_sample");
    auto input = file_to_vec<std::string>("input_actual");

    auto col_size = input[0].size();
    auto row_size = input.size();

    auto gears = std::vector<num_gear>{};
    std::uint32_t sum{0};

    for(auto row = 0; row < row_size; ++row)
    {
        for(auto col = 0; col < col_size; ++col)
        {
            if(!isdigit(input[row][col]))
            {
                continue;
            }

            // Find size of number
            auto num_end = col;
            while( num_end< col_size && isdigit(input[row][num_end]))
            {
                num_end++;
            }
            auto num_size = num_end - col;

            // Look for gears
            std::size_t g_col{0};
            std::size_t g_row{0};
            if(has_gear(input, col, num_size, row, g_col, g_row))
            {
                auto new_gear = num_gear{std::stoi(input[row].substr(col, num_size)), g_col, g_row};
                auto g = std::find(gears.begin(), gears.end(), new_gear);
                if(g != gears.end())
                {
                    sum += g->num * new_gear.num;
                }
                gears.emplace_back(new_gear);
            }
            col += num_size;
        }
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