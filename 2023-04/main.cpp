#include "../tools/tools.h"
#include "../tools/types.h"

#include <cstdint>
#include <iostream>

class scrach_game{
public:
    std::uint32_t id;
    std::vector<std::uint32_t> nums;
    std::vector<std::uint32_t> wins;

    friend std::istream &operator>> ( std::istream  &input, scrach_game &g ) {
        std::string val;
        std::getline(input, val);

        auto base = str_to_vec<std::string>(val, ":");
        auto id = string_to<str_num>(base[0]);
        g.id = id.get_num();

        auto sides = str_to_vec<std::string>(base[1], "|");
        g.wins = str_to_vec<uint32_t>(sides[0], " ");
        g.nums = str_to_vec<uint32_t>(sides[1], " ");
        return input;           
    };

    std::uint32_t simple_score()
    {
        std::uint32_t sum{0};
        for( auto &&n : nums){
            for( auto &&w : wins){
                if(n == w) {
                    if(sum == 0) {
                        sum = 1;
                    } else {
                        sum = sum * 2;
                    }
                    break;
                }
            }
        }
        std::cout<< sum << "\n";
        return sum;
    }
};

void part1()
{
    auto games = file_to_vec<scrach_game>("input_sample");
    std::uint32_t sum = 0;
    for( auto &&g : games)
    {
        sum += g.simple_score();
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