#include "../tools/tools.h"
#include "../tools/types.h"

#include <cstdint>
#include <iostream>

constexpr char NOT_DIGIT = ' ';

std::uint32_t num_from_line(std::string & line)
{
    char first = NOT_DIGIT;
    for(auto &&c : line)
    {
        if(isdigit(c))
        {
            first = c;
            break;
        }
    }

    char last = NOT_DIGIT;
    for(auto c = line.end(); c >= line.begin(); c--)
    {
        if(isdigit(*c))
        {
            last = *c;
            break;
        }
    }
    return stoi(std::string{first, last});
}

char val_at_pos(std::string const & line, std::string::iterator const & iter, std::string const & val, char return_val)
{
    if(line.substr(iter - line.begin(), val.size()) == val)
    {
        return return_val;
    }
    return NOT_DIGIT;
}

char get_num_str(std::string const & line, std::string::iterator & pos)
{
    char ret_val = NOT_DIGIT;
    ret_val = val_at_pos(line, pos, "one", '1');
    if(ret_val != NOT_DIGIT)
    {
        return ret_val;
    }
    ret_val = val_at_pos(line, pos, "two", '2');
    if(ret_val != NOT_DIGIT)
    {
        return ret_val;
    }
    ret_val = val_at_pos(line, pos, "three", '3');
    if(ret_val != NOT_DIGIT)
    {
        return ret_val;
    }
    ret_val = val_at_pos(line, pos, "four", '4');
    if(ret_val != NOT_DIGIT)
    {
        return ret_val;
    }
    ret_val = val_at_pos(line, pos, "five", '5');
    if(ret_val != NOT_DIGIT)
    {
        return ret_val;
    }
    ret_val = val_at_pos(line, pos, "six", '6');
    if(ret_val != NOT_DIGIT)
    {
        return ret_val;
    }
    ret_val = val_at_pos(line, pos, "seven", '7');
    if(ret_val != NOT_DIGIT)
    {
        return ret_val;
    }
    ret_val = val_at_pos(line, pos, "eight", '8');
    if(ret_val != NOT_DIGIT)
    {
        return ret_val;
    }
    ret_val = val_at_pos(line, pos, "nine", '9');
    return ret_val;
}

char check_pos(std::string const & line, std::string::iterator & pos)
{
    if(isdigit(*pos))
    {
        return *pos;
    }
    else
    {
        auto val = get_num_str(line, pos);
        if(val != NOT_DIGIT)
        {
            return val;
        }
    }
    return NOT_DIGIT;
}

std::uint32_t num_from_line_str(std::string & line)
{
    char first = NOT_DIGIT;
    for(auto c = line.begin(); c < line.end(); c++)
    {
        auto val = check_pos(line, c);
        if (val != NOT_DIGIT)
        {
            first = val;
            break;
        }
    }

    char last = NOT_DIGIT;
    for(auto c = line.end(); c >= line.begin(); c--)
    {
        auto val = check_pos(line, c);
        if (val != NOT_DIGIT)
        {
            last = val;
            break;
        }
    }
    return stoi(std::string{first, last});
}

void part1()
{
    auto lines = file_to_vec<std::string>("input_actual");
    std::uint32_t sum = 0;
    for(auto &&line : lines)
    {
        sum += num_from_line(line);
    }
    std::cout << "Sum = " << sum << "\n";
}

void part2()
{
    auto lines = file_to_vec<std::string>("input_actual");
    std::uint32_t sum = 0;
    for(auto &&line : lines)
    {
        sum += num_from_line_str(line);
    }
    std::cout << "Sum = " << sum << "\n";
}

int main(int argc, char* argv[])
{
    std::cout << "---- Part1 ----\n";
    part1();
    std::cout << "---- Part2 ----\n";
    part2();
}