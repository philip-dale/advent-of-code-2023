#include "../tools/tools.h"
#include "../tools/types.h"

#include <cstdint>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <format>

void part1()
{
    auto lines = file_to_vec<std::string>("input_actual");
    auto time_line = str_to_vec<std::string>(lines[0], ":");
    auto dist_line = str_to_vec<std::string>(lines[1], ":");
    auto times = str_to_vec<double>(time_line[1], " ");
    auto dists = str_to_vec<double>(dist_line[1], " ");

    double sum{1};
    for(auto i=0; i<times.size(); i++)
    {
        auto vals = solve_quad(1, times[i]*-1, dists[i]+1);
        sum *= std::abs(std::floor(vals[0]) - std::ceil(vals[1])) + 1;
    }
    std::cout << std::format("{}",sum) << "\n";
}

void part2()
{
    auto lines = file_to_vec<std::string>("input_actual");
    auto time_line = str_to_vec<std::string>(lines[0], ":");
    auto dist_line = str_to_vec<std::string>(lines[1], ":");

    auto time = stod(remove_space(time_line[1]));
    auto dist = stod(remove_space(dist_line[1]));

    auto vals = solve_quad(1, time*-1, dist+1);
    auto sum = std::abs(std::floor(vals[0]) - std::ceil(vals[1])) + 1;
    std::cout << std::format("{}",sum) << "\n";
}

int main(int argc, char* argv[])
{
    std::cout << "---- Part1 ----\n";
    part1();
    std::cout << "---- Part2 ----\n";
    part2();
}