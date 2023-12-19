#include "../tools/tools.h"
#include "../tools/types.h"

#include <cstdint>
#include <iostream>
#include <regex>
#include <map>

struct part{
    std::int64_t x;
    std::int64_t m;
    std::int64_t a;
    std::int64_t s;
    std::int64_t get(std::string & v)
    {
        if(v == "x")
        {
            return x;
        }
        if(v == "m")
        {
            return m;
        }
        if(v == "a")
        {
            return a;
        }
        return s;
    }
};

class instruction{
public:
    instruction() : 
        categorie{},
        greater{false},
        val{0},
        result{}
    {}

    instruction(std::string const& input)
    {
        auto parts = std::regex("(.)(.)([^:])(.+)");
        std::smatch matches;
        std::regex_search(input, matches, parts);
        categorie = matches[1].str();
        greater = matches[2].str() == ">";
        val = stoi(matches[3].str());
        result = matches[4].str();
    }

    bool process(part & p)
    {
        auto input = p.get(categorie);
        if(greater && input > val)
        {
            return true;
        }
        else if(!greater && input < val)
        {
            return true;
        }
        return false;
    }

    std::string categorie;
    bool greater;
    std::int64_t val;
    std::string result;
};

class instruction_set{
public:
    instruction_set() : 
        code{},
        instructions{},
        fail_val{}
    {};

    instruction_set(std::string const& input)
    {
        auto parts = std::regex("([^{]+)\\{([^}]+)\\}");
        std::smatch matches;
        std::regex_search(input, matches, parts);
        code = matches[1].str();
        auto inst = str_to_vec<std::string>(matches[2].str(), ",");
        for(auto i=0; i<inst.size()-2; ++i)
        {
            instructions.emplace_back(instruction(inst[i]));
        }
        fail_val = inst[inst.size()-1];
    }

    std::string process(part & p)
    {
        for(auto i=0; i<instructions.size(); ++i)
        {
            if(instructions[i].process(p))
            {
                return instructions[i].result;
            }
        }
        return fail_val;
    }

    std::string code;
    std::vector<instruction> instructions;
    std::string fail_val;
};



void part1()
{
    auto sections = file_to_vec<std::string>("input_sample", std::string(NEW_LINE) + std::string(NEW_LINE));
    auto instruction_lines = str_to_vec<std::string>(sections[0]);
    auto processes = std::map<std::string, instruction_set>{};
    for(auto &&il : instruction_lines)
    {
        auto is = instruction_set(il);
        processes[is.code] = std::move(is);
    }

    auto parts = std::vector<part>{};
    auto part_lines = str_to_vec<std::string>(sections[1]);
    for(auto &&pl : instruction_lines)
    {
        auto prop_regex = std::regex("\\{x=([^,]+),m=([^,]+),a=([^,]+),s=([^,]+)\\}");
        std::smatch matches;
        std::regex_search(pl, matches, prop_regex);
        parts.emplace_back(part{stoi(matches[1].str()),stoi(matches[2].str()),stoi(matches[3].str()),stoi(matches[4].str())});
    }

    auto sum = std::int64_t{0};
    
    for(auto &&p : parts)
    {
        auto current_inst = std::string("in");
        while(current_inst != "A" || current_inst != "R")
        {
            current_inst = processes[current_inst].process(p);
        }
        if(current_inst == "A")
        {
            sum += p.a + p.m + p.s + p.x;
        }
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