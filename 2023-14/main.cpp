#include "../tools/tools.h"
#include "../tools/types.h"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <map>

enum class direction{
    north = 0,
    south,
    east,
    west,
};

void tilt_north(std::vector<std::string> & board)
{
    for(auto r=1; r<board.size(); ++r)
    {
        for(auto c=0; c<board[r].size(); ++c)
        {
            if(board[r][c] == 'O')
            {
                std::int64_t r_m = r-1;
                while(r_m >= 0 && board[r_m][c] == '.')
                {
                    r_m--;
                }
                board[r][c] = '.';
                board[r_m+1][c] = 'O';
            }
        }
    }
}

void tilt_south(std::vector<std::string> & board)
{
    for(std::int64_t r=board.size()-2; r>=0; --r)
    {
        for(auto c=0; c<board[r].size(); ++c)
        {
            if(board[r][c] == 'O')
            {
                std::int64_t r_m = r+1;
                while(r_m < board.size() && board[r_m][c] == '.')
                {
                    r_m++;
                }
                board[r][c] = '.';
                board[r_m-1][c] = 'O';
            }
        }
    }
}

void tilt_west(std::vector<std::string> & board)
{
    for(auto c=1; c<board[0].size(); ++c)
    {
        for(auto r=0; r<board.size(); ++r)
        {
            if(board[r][c] == 'O')
            {
                std::int64_t c_m = c-1;
                while(c_m >= 0 && board[r][c_m] == '.')
                {
                    c_m--;
                }
                board[r][c] = '.';
                board[r][c_m+1] = 'O';
            }
        }
    }
}

void tilt_east(std::vector<std::string> & board)
{
    for(std::int64_t c=board[0].size()-2; c>=0; --c)
    {
        for(auto r=0; r<board.size(); ++r)
        {
            if(board[r][c] == 'O')
            {
                std::int64_t c_m = c+1;
                while(c_m < board[0].size() && board[r][c_m] == '.')
                {
                    c_m++;
                }
                board[r][c] = '.';
                board[r][c_m-1] = 'O';
            }
        }
    }
}

void tilt_all(std::vector<std::string> & board)
{
    tilt_north(board);
    tilt_west(board);
    tilt_south(board);
    tilt_east(board);
}

std::int64_t get_weight(std::vector<std::string> & board)
{
    auto sum = std::int64_t{0};
    for(auto r=0; r<board.size(); ++r)
    {
        auto rocks = std::int64_t{0};
        for(auto &&c : board[r])
        {
            if(c == 'O')
            {
                rocks++;
            }
        }
        sum += rocks * (board.size() - r);
    }
    return sum;
}

std::string board_string(std::vector<std::string> & a)
{
    auto ret=std::string{};
    for(auto r=0; r < a.size(); ++r)
    {
        ret += a[r];
    }
    return ret;
}

void part1()
{
    auto board = file_to_vec<std::string>("input_actual");
    tilt_north(board);
    std::cout << get_weight(board) << "\n";
}

void part2()
{
    auto board = file_to_vec<std::string>("input_actual");
    auto cache = std::vector<std::string>{};
    auto loop_count = std::uint64_t{1000000000};
    auto remaining_mod = std::uint64_t{0};

    for(auto i=0; i<loop_count; ++i)
    {
        auto board_key = board_string(board);
        auto match = std::find(cache.begin(), cache.end(), board_key);
        if(match != cache.end())
        {
            // repeat found;
            auto loop_size = cache.size() - (match - cache.begin());
            auto remaing_steps = loop_count - i;
            remaining_mod = remaing_steps % loop_size;
            break;
        }
        cache.emplace_back(board_key);
        tilt_all(board);
    }
    
    for(auto i=0; i<remaining_mod; ++i)
    {
        tilt_all(board);
    }

    std::cout << get_weight(board) << "\n";
}

int main(int argc, char* argv[])
{
    std::cout << "---- Part1 ----\n";
    part1();
    std::cout << "---- Part2 ----\n";
    part2();
}