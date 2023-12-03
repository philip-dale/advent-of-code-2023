#include "../tools/tools.h"
#include "../tools/types.h"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <optional>

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

struct num_gear{
    std::int32_t num;
    std::size_t col;
    std::size_t row;

    bool operator==(const num_gear& b)
    {
        return col == b.col && row == b.row;
    }
};


std::optional<num_gear> has_gear(std::vector<std::string> const & input, std::size_t num_start, std::size_t num_size, std::size_t num_row)
{
    auto box = get_bounding_box<std::string>(input, area{num_row, num_start, num_row, num_start + num_size-1}, 1);
    
    for(auto col = box.col_start; col <= box.col_end; ++col) 
    {
        for(auto row = box.row_start; row <= box.row_end; ++row) 
        {
            if(row == num_row && ( col > box.col_start && col < box.col_end ) )
            {
                continue;
            }
            char current_char = input[row][col];
            if(current_char == '*')
            {
                return num_gear{std::stoi(input[num_row].substr(num_start, num_size)), col, row};
            }
        }
    }
    return {};
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
            auto new_gear = has_gear(input, col, num_size, row);
            if(new_gear)
            {
                auto g = std::find(gears.begin(), gears.end(), new_gear.value());
                if(g != gears.end())
                {
                    sum += g->num * new_gear.value().num;
                }
                gears.emplace_back(std::move(new_gear.value()));
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