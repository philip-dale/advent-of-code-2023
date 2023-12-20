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
    std::int64_t get(std::string & c)
    {
        if(c == "x")
        {
            return x;
        }
        if(c == "m")
        {
            return m;
        }
        if(c == "a")
        {
            return a;
        }
        return s;
    }

    void set(std::string & c, std::int64_t v)
    {
        if(c == "x")
        {
            x = v;
            return;
        }
        if(c == "m")
        {
            m = v;
            return;
        }
        if(c == "a")
        {
            a = v;
            return;
        }
        s = v;
    }

    void print()
    {
        std::cout << x << ", " << m << ", " << a << ", " << s << "\n";
    }
};

struct part_range{
    part min;
    part max;
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
        auto parts = std::regex("(.)(.)([^:]+):(.+)");
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

    bool process(part_range & pr)
    {  
        auto min = pr.min.get(categorie);
        auto max = pr.max.get(categorie);

        if(greater && min > val)
        {
            return true;
        }
        else if(!greater && max < val)
        {
            return true;
        }
        return false;
    }

    void modify_range(part_range & a, part_range & b)
    {
        if(greater)
        {
            a.min.set(categorie, val+1);
            b.max.set(categorie, val);
        }
        else
        {
            a.max.set(categorie, val-1);
            b.min.set(categorie, val);
        }
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
        for(auto i=0; i<inst.size()-1; ++i)
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
    auto sections = file_to_vec<std::string>("input_actual", std::string(NEW_LINE) + std::string(NEW_LINE));
    auto instruction_lines = str_to_vec<std::string>(sections[0]);
    auto processes = std::map<std::string, instruction_set>{};
    for(auto &&il : instruction_lines)
    {
        auto is = instruction_set(il);
        processes[is.code] = std::move(is);
    }

    auto parts = std::vector<part>{};
    auto part_lines = str_to_vec<std::string>(sections[1]);
    for(auto &&pl : part_lines)
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
        while(current_inst != "A" && current_inst != "R")
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

void walk_range(std::map<std::string, instruction_set> & processes, std::string const& process, part_range range, std::vector<part_range> & posibles)
{
    if(process == "A")
    {
        posibles.emplace_back(range);
        return;
    }

    if(process == "R")
    {
        return;
    }

    for(auto i=0; i<processes[process].instructions.size(); ++i)
    {
        auto match = processes[process].instructions[i].process(range);
        if(match)
        {
            walk_range(processes, processes[process].instructions[i].result, range, posibles);
            break;
        }
        else
        {
            // adjust range and cary on
            auto range_a = range;
            // auto range_b = range;
            processes[process].instructions[i].modify_range(range_a, range);
            walk_range(processes, processes[process].instructions[i].result, range_a, posibles);
            if(i == processes[process].instructions.size()-1)
            {
                walk_range(processes, processes[process].fail_val, range, posibles);
                break;
            }
        }
    }
}

void part2()
{
    auto sections = file_to_vec<std::string>("input_sample", std::string(NEW_LINE) + std::string(NEW_LINE));
    auto instruction_lines = str_to_vec<std::string>(sections[0]);
    auto processes = std::map<std::string, instruction_set>{};
    for(auto &&il : instruction_lines)
    {
        auto is = instruction_set(il);
        processes[is.code] = std::move(is);
    }

    auto start_range = part_range{{1,1,1,1},{4000,4000,4000,4000}};
    std::vector<part_range> posibles{};
    walk_range(processes, "in", start_range, posibles);

    // auto totals = posibles[0];
    // for(auto i=1; i<posibles.size(); ++i)
    // {

    // }

    for(auto &&result : posibles)
    {
        std::cout << result.min.x << " " << result.max.x << "\t\t\t" << result.min.m << " " << result.max.m << "\t\t\t" << result.min.a << " " << result.max.a << "\t\t\t" << result.min.s << " " << result.max.s << "\n";
        // std::cout << (result.max.x - result.min.x + 1) * (result.max.m - result.min.m + 1) * (result.max.a - result.min.a + 1) * (result.max.s - result.min.s + 1) << "\n";
        // std::cout << (result.max.x - result.min.x + 1) << " " << (result.max.m - result.min.m + 1) << " " << (result.max.a - result.min.a + 1) << " " << (result.max.s - result.min.s + 1) << "\n";
    }
}

int main(int argc, char* argv[])
{
    // std::cout << "---- Part1 ----\n";
    // part1();
    std::cout << "---- Part2 ----\n";
    part2();
}