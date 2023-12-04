#include "../tools/tools.h"
#include "../tools/types.h"

#include <cstdint>
#include <iostream>

class scrach_game{
public:
    std::uint32_t id;
    std::vector<std::uint32_t> nums;
    std::vector<std::uint32_t> wins;
    std::uint32_t score;
    std::uint32_t win_count;

    friend std::istream &operator>> ( std::istream  &input, scrach_game &g ) {
        std::string val;
        std::getline(input, val);

        auto base = str_to_vec<std::string>(val, ":");
        auto id = string_to<str_num>(base[0]);
        g.id = id.get_num();

        auto sides = str_to_vec<std::string>(base[1], "|");
        
        g.wins = str_to_vec<uint32_t>(sides[0], " ");
        g.nums = str_to_vec<uint32_t>(sides[1], " ");

        g.calc_score();
        return input;           
    };

    void calc_score()
    {
        score = 0;
        win_count = 0;
        for( auto &&n : nums){
            for( auto &&w : wins){
                if(n == w) {
                    if(score == 0) {
                        score = 1;
                    } else {
                        score = score * 2;
                    }
                    win_count += 1;
                    break;
                }
            }
        }
    }
};

void count_cards(std::vector<scrach_game> & games, std::vector<std::uint32_t> & counts, std::uint32_t id)
{
    counts[id] += 1;
    for(auto i = 1; i <= games[id].win_count; ++i)
    {
        if(i + id >= counts.size())
        {
            return;
        }
        count_cards(games, counts, i + id);
    }
}

void part1()
{
    auto games = file_to_vec<scrach_game>("input_actual");
    std::uint32_t sum = 0;
    for( auto &&g : games)
    {
        sum += g.score;
    }
    std::cout << sum << "\n";
}

void part2()
{
    auto games = file_to_vec<scrach_game>("input_actual");
    auto counts = std::vector<std::uint32_t>(games.size(), 0);
    
    for( auto &&g : games)
    {
        count_cards(games, counts, g.id-1);
    }
    auto sum = std::uint32_t{0};
    for( auto &&c : counts)
    {
        sum += c;
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