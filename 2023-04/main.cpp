#include "../tools/tools.h"
#include "../tools/types.h"

#include <cstdint>
#include <iostream>

class scrach_game{
public:
    scrach_game() :
        m_id{0},
        m_score{0},
        m_win_count{0},
        m_wins{},
        m_nums{}
    {};

    scrach_game(std::uint32_t id, std::string & wins, std::string & nums) : 
        m_id{id},
        m_score{0},
        m_win_count{0}
    {
        m_wins = str_to_vec<std::uint32_t>(wins, " ");
        m_nums = str_to_vec<std::uint32_t>(nums, " ");
        calc_score();
    };

    friend std::istream &operator>> ( std::istream  &input, scrach_game &g ) {
        std::string val;
        std::getline(input, val);

        auto base = str_to_vec<std::string>(val, ":");
        auto id = string_to<str_num>(base[0]);
        auto sides = str_to_vec<std::string>(base[1], "|");
        
        g = scrach_game(id.get_num(), sides[0], sides[1]);
        return input;           
    };

    std::uint32_t get_id()
    {
        return m_id;
    };

    std::uint32_t get_score()
    {
        return m_score;
    };

    std::uint32_t get_win_count()
    {
        return m_win_count;
    };

private:
    void calc_score()
    {
        m_score = 0;
        m_win_count = 0;
        for( auto &&n : m_nums){
            for( auto &&w : m_wins){
                if(n == w) {
                    if(m_score == 0) {
                        m_score = 1;
                    } else {
                        m_score = m_score * 2;
                    }
                    m_win_count += 1;
                    break;
                }
            }
        }
    }

    std::uint32_t m_id;
    std::uint32_t m_score;
    std::uint32_t m_win_count;
    std::vector<std::uint32_t> m_nums;
    std::vector<std::uint32_t> m_wins;
};

void count_cards(std::vector<scrach_game> & games, std::vector<std::uint32_t> & counts, std::uint32_t id)
{
    counts[id] += 1;
    for(auto i = 1; i <= games[id].get_win_count(); ++i)
    {
        if(i + id >= counts.size())
        {
            return;
        }
        count_cards(games, counts, i + id);
    }
}

void part1()
{
    auto games = file_to_vec<scrach_game>("input_actual");
    std::uint32_t sum = 0;
    for( auto &&g : games)
    {
        sum += g.get_score();
    }
    std::cout << sum << "\n";
}

void part2()
{
    auto games = file_to_vec<scrach_game>("input_actual");
    auto counts = std::vector<std::uint32_t>(games.size(), 0);
    
    for( auto &&g : games)
    {
        count_cards(games, counts, g.get_id()-1);
    }
    auto sum = std::uint32_t{0};
    for( auto &&c : counts)
    {
        sum += c;
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