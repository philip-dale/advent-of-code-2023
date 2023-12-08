#include "../tools/tools.h"
#include "../tools/types.h"

#include <cstdint>
#include <iostream>
#include <map>
#include <regex>
#include <numeric>
#include <cmath>

typedef std::pair<std::string, std::string> NodePair;
typedef std::map<std::string, NodePair> NodeMap;

class node_game {
public:
    std::string instructions;
    NodeMap nodes;

    node_game(std::string const & filename)
    {
        auto lines = file_to_vec<std::string>(filename);
        instructions = lines[0];

        nodes = NodeMap{};
        for(auto i=0; i<lines.size(); ++i)
        {
            auto parts = std::regex("(...) = \\((...), (...)\\)");
            std::smatch matches;
            std::regex_search(lines[i], matches, parts);

            nodes[matches[1].str()] = NodePair(matches[2].str(), matches[3].str());
        }
    };
    
    std::size_t count_steps(std::string const & starting_node, std::string const & end_node)
    {
        auto steps = std::size_t{0};
        auto instruction_index = std::size_t{0};
        auto current_key = starting_node;
        while(!current_key.ends_with(end_node))
        {
            if(instructions[instruction_index] == 'L')
            {
                current_key = nodes[current_key].first;
            }
            else
            {
                current_key = nodes[current_key].second;
            }
            steps += 1;
            instruction_index += 1;
            if(instruction_index >= instructions.size())
            {
                instruction_index = 0;
            }
        }
        return steps;
    }

};

void part1()
{
    auto game = node_game("input_actual");
    auto steps = game.count_steps("AAA", "ZZZ");
    std::cout << steps << "\n";
}

void part2()
{
    auto game = node_game("input_actual");
    
    std::vector<std::string> current_nodes{};
    for(auto &&n : game.nodes)
    {
        if(n.first[2] == 'A')
        {
            current_nodes.emplace_back(n.first);
        }
    }
    auto steps = std::vector<std::size_t>{};
    for(auto && cn : current_nodes)
    {
        steps.emplace_back(game.count_steps(cn, "Z"));
    }

    auto result = lcm(steps);
        
    std::cout << result << "\n";
}

int main(int argc, char* argv[])
{
    std::cout << "---- Part1 ----\n";
    part1();
    std::cout << "---- Part2 ----\n";
    part2();
}