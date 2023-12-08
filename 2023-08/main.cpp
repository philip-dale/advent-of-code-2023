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
    

};

void part1()
{
    auto game = node_game("input_actual");

    auto steps = std::size_t{0};
    auto instruction_index = std::size_t{0};
    auto current_key = std::string("AAA");
    while(current_key != "ZZZ")
    {
        if(game.instructions[instruction_index] == 'L')
        {
            current_key = game.nodes[current_key].first;
        }
        else
        {
            current_key = game.nodes[current_key].second;
        }
        steps += 1;
        instruction_index += 1;
        if(instruction_index >= game.instructions.size())
        {
            instruction_index = 0;
        }
    }
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
    auto all_steps = std::vector<std::size_t>{};


    for(auto && cn : current_nodes)
    {
        auto steps = std::size_t{0};
        auto instruction_index = std::size_t{0};
        auto current_key = cn;
        while(current_key[2] != 'Z')
        {
            if(game.instructions[instruction_index] == 'L')
            {
                current_key = game.nodes[current_key].first;
            }
            else
            {
                current_key = game.nodes[current_key].second;
            }
            steps += 1;
            instruction_index += 1;
            if(instruction_index >= game.instructions.size())
            {
                instruction_index = 0;
            }
        }
        all_steps.emplace_back(steps);
    }

    // Use Chinese remainder theorem to get lowest common multiple
    auto result = lcm(all_steps);
        
    std::cout << result << "\n";
}

int main(int argc, char* argv[])
{
    std::cout << "---- Part1 ----\n";
    part1();
    std::cout << "---- Part2 ----\n";
    part2();
}