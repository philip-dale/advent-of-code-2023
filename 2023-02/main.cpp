#include "../tools/tools.h"
#include "../tools/types.h"

#include <cstdint>
#include <iostream>
#include <regex>

class cube_game {
public:
    cube_game():
        id{0},
        blue_max{0},
        red_max{0},
        green_max{0}
    {};

    std::uint32_t id;
    std::uint32_t blue_max;
    std::uint32_t red_max;
    std::uint32_t green_max;

    friend std::ostream &operator<< ( std::ostream &output, const cube_game &g ) { 
        output << "ID = " << g.id << ", blue = " << g.blue_max << ", red = " << g.red_max << ", green = " << g.green_max << "\n";
        return output;            
    };

    friend std::istream &operator>> ( std::istream  &input, cube_game &g ) {
        std::string val;
        std::getline(input, val);

        auto base = str_to_vec<std::string>(val, ":");
        auto id = string_to<str_num>(base[0]);

        g.id = id.get_num();

        auto hands = str_to_vec<std::string>(base[1], ";");
        for(auto &&hand : hands)
        {
            auto pulls = str_to_vec<num_str>(hand, ",");
            for(auto &&pull : pulls)
            {
                auto colour = pull.get_str();
                auto count = pull.get_num();
                if(colour == "blue")
                {
                    if(count > g.blue_max)
                    {
                        g.blue_max = count;
                    }
                }
                if(colour == "red")
                {
                    if(count > g.red_max)
                    {
                        g.red_max = count;
                    }
                }
                if(colour == "green")
                {
                    if(count > g.green_max)
                    {
                        g.green_max = count;
                    }
                }
            }
        }
        return input;           
    };
};

void part1()
{
    // auto games = file_to_vec<cube_game>("input_sample");
    auto games = file_to_vec<cube_game>("input_actual");

    std::uint32_t red{12};
    std::uint32_t green{13};
    std::uint32_t blue{14};
    std::uint32_t sum{0};

    for(auto &&g : games)
    {
        if(g.blue_max <= blue && g.green_max <= green && g.red_max <= red)
        {
            sum += g.id;
        }
    }
    std::cout << sum << "\n";
}

void part2()
{
    // auto games = file_to_vec<cube_game>("input_sample");
    auto games = file_to_vec<cube_game>("input_actual");

    std::uint32_t sum{0};

    for(auto &&g : games)
    {
        sum += g.red_max * g.blue_max * g.green_max;
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