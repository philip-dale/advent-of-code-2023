#include "../tools/tools.h"
#include "../tools/types.h"

#include <cstdint>
#include <iostream>

int main(int argc, char* argv[])
{
    // Points
    auto points = std::string("1,2" + std::string(NEW_LINE) + "3,4" + std::string(NEW_LINE) + "5,6" + std::string(NEW_LINE) + "7,8" + std::string(NEW_LINE));
    auto point_vec = str_to_vec<point<std::uint64_t>>(points);
    print_vec(point_vec);

    // Lines
    auto lines = std::vector<line<std::uint64_t>>{{point_vec[0], point_vec[1]}, {point_vec[2], point_vec[3]}};
    print_vec(lines);

    //Pairs
    auto pair_str = std::string("A X" + std::string(NEW_LINE) + "B Y" + std::string(NEW_LINE) + "C Z" + std::string(NEW_LINE) + "D Q" + std::string(NEW_LINE));
    auto pairs = str_to_vec_sep<pair<char>>(pair_str, " ");
    print_vec(pairs);

    // 2d array
    auto data_2d_str = std::string("1,2,3,4,5" + std::string(NEW_LINE) + "6,7,8,9,10" + std::string(NEW_LINE) + "11,12,13,14,15" + std::string(NEW_LINE) );
    auto data_2d = str_to_2d<std::string>(data_2d_str, ",");
    print_2d(data_2d, 5, '-');
}   