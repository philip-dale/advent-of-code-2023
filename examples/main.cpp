#include "../tools/tools.h"

#include <cstdint>
#include <iostream>

int main(int argc, char* argv[])
{
    // Points
    std::string points = std::string("1,2" + std::string(NEW_LINE) + "3,4" + std::string(NEW_LINE) + "5,6" + std::string(NEW_LINE) + "7,8");
    auto point_vec = str_to_vec<point<std::uint64_t>>(points);
    print_vec(point_vec);

    // Lines
    auto lines = std::vector<line<std::uint64_t>>{{point_vec[0], point_vec[1]}, {point_vec[2], point_vec[3]}};
    print_vec(lines);
}