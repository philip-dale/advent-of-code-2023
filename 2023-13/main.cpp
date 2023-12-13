#include "../tools/tools.h"
#include "../tools/types.h"

#include <cstdint>
#include <iostream>

std::int64_t find_mirror(std::vector<std::string> & data)
{
    auto row = std::size_t(1);
    for(;row<data.size(); row++)
    {
        std::int64_t u_pos = row-1;
        std::int64_t d_pos = row;
        bool found = true;
        while(u_pos >= 0 && d_pos < data.size())
        {
            if(data[u_pos] != data[d_pos])
            {
                found = false;
                break;
            }
            u_pos--;
            d_pos++;
        }
        if(found)
        {
            return row;
        }
    }
    return data.size();
}

std::int64_t string_differences(std::string & a, std::string & b)
{
    auto count = std::int64_t{0};
    for(auto i=0; i<a.size(); ++i)
    {
        if(a[i] != b[i])
        {
            count++;
        }
    }
    return count;
}

std::int64_t find_mirror_smudge(std::vector<std::string> & data)
{
    auto row = std::size_t(1);
    for(;row<data.size(); row++)
    {
        std::int64_t u_pos = row-1;
        std::int64_t d_pos = row;
        bool found = true;
        auto diff_count = std::int64_t{0};
        while(u_pos >= 0 && d_pos < data.size())
        {
            diff_count += string_differences(data[u_pos], data[d_pos]);
            if(diff_count > 1)
            {
                found = false;
                break;
            }
            u_pos--;
            d_pos++;
        }
        if(found && diff_count == 1)
        {
            return row;
        }
    }
    return data.size();
}

void part1()
{
    auto boards = file_to_vec<std::string>("input_actual", std::string(NEW_LINE) + std::string(NEW_LINE));
    auto sum = std::uint64_t{0};
    for(auto &&b : boards)
    {
        auto rows = str_to_vec<std::string>(b);

        auto cols = std::vector<std::string>(rows[0].size(), "");
        for(auto r=0; r<rows.size(); r++)
        {
            for(auto c=0; c<cols.size(); c++)
            {
                cols[c] += (rows[r][c]);
            }
        }
        auto r_pos = find_mirror(rows);
        if(r_pos != rows.size())
        {
            sum += (100 * r_pos);
            continue;
        }
        auto c_pos = find_mirror(cols);
        if(c_pos != cols.size())
        {
            sum += c_pos;
        }
        
    }
    std::cout << sum << "\n";
}

void part2()
{
    auto boards = file_to_vec<std::string>("input_actual", std::string(NEW_LINE) + std::string(NEW_LINE));
    auto sum = std::uint64_t{0};
    for(auto &&b : boards)
    {
        auto rows = str_to_vec<std::string>(b);

        auto cols = std::vector<std::string>(rows[0].size(), "");
        for(auto r=0; r<rows.size(); r++)
        {
            for(auto c=0; c<cols.size(); c++)
            {
                cols[c] += (rows[r][c]);
            }
        }
        auto r_pos = find_mirror_smudge(rows);
        if(r_pos != rows.size())
        {
            sum += (100 * r_pos);
            continue;
        }
        auto c_pos = find_mirror_smudge(cols);
        if(c_pos != cols.size())
        {
            sum += c_pos;
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