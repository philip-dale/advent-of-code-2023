#include "../tools/tools.h"
#include "../tools/types.h"

#include <cstdint>
#include <iostream>
#include <map>
#include <algorithm>

bool JOKERS_WILD = false;

std::uint32_t card_lookup(char c)
{
    switch(c)
    {
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5; 
        case '6':
            return 6;
        case '7':
            return 7; 
        case '8':
            return 8;
        case '9':
            return 9; 
        case 'T':
            return 10;
        case 'J':
            if(JOKERS_WILD)
            {
                return 1;
            }
            return 11; 
        case 'Q':
            return 12;
        case 'K':
            return 13;
        case 'A':
            return 14;
        default:
            std::cout <<"I SHOULD NOT BE HERE " << c << "\n";
    }
    return 0;
}

class hand {
public:
    hand() :
        m_cards{},
        m_scores{},
        m_bet{}
    {};

    friend std::istream &operator>> ( std::istream  &input, hand &g ) {
        std::string val;
        std::getline(input, val);
        auto parts = str_to_vec<std::string>(val, " ");
        g.m_cards = parts[0];
        g.m_bet = stoi(parts[1]);

        // Figure out what type of hand we have
        auto card_counts = std::map<char, std::uint32_t>{};
        auto jokers_count = std::uint32_t{0};
        for(auto &&c : g.m_cards)
        {
            if(c == 'J')
            {
                jokers_count += 1;
            }
            if(card_counts.contains(c))
            {
                card_counts[c] = card_counts[c] + 1;
            }
            else
            {
                card_counts[c] = 1;
            }
        }
        
        auto sorted_cards = std::vector<std::pair<char, std::uint32_t>>{};
        for(auto &&c : card_counts)
        {
            sorted_cards.emplace_back(std::pair(c.first, c.second));
        }
        std::sort(sorted_cards.begin(), sorted_cards.end(), [](std::pair<char, std::uint32_t> & a, std::pair<char, std::uint32_t> & b)
        {
            return a.second > b.second;
        });

        if(JOKERS_WILD)
        {
            if(sorted_cards[0].first == 'J')
            {
                sorted_cards[1].second += jokers_count;
            }
            else
            {
                sorted_cards[0].second += jokers_count;
            }
            
            if(sorted_cards.size() > 1 && jokers_count > 0)
            {
                auto jokers_index = std::size_t{0};
                for(auto i=0; i<sorted_cards.size(); i++)
                {
                    if(sorted_cards[i].first == 'J')
                    {
                        jokers_index = i;
                        break;
                    }
                }
                sorted_cards.erase(sorted_cards.begin() + jokers_index);
            }
        }

        switch(sorted_cards.size())
        {
            case 1:
                g.m_scores.emplace_back(6);
                break;
            case 2:
                // full house or 4 of a kind
                if(sorted_cards[0].second == 4)
                {
                    g.m_scores.emplace_back(5);
                }
                else
                {
                    g.m_scores.emplace_back(4);
                }
                break;
            case 3:
                // three of a kind, 2 pair
                // full house or 4 of a kind
                if(sorted_cards[0].second == 3)
                {
                    g.m_scores.emplace_back(3);
                }
                else
                {
                    g.m_scores.emplace_back(2);
                }
                break;
            case 4:
                g.m_scores.emplace_back(1);
                break;
            case 5:
                g.m_scores.emplace_back(0);
                break;
        }
        for(auto && c : g.m_cards)
        {
            g.m_scores.emplace_back(card_lookup(c));
        }

        return input;           
    };

    std::vector<std::uint32_t> &scores()
    {
        return m_scores;
    };

    std::uint32_t bet()
    {
        return m_bet;
    };
private:
    std::string m_cards;
    std::vector<std::uint32_t> m_scores;
    std::uint32_t m_bet;

};

void play()
{
    auto hands = file_to_vec<hand>("input_actual");
    std::sort(hands.begin(), hands.end(), [](hand & a, hand & b){
        for(auto i=0; i<a.scores().size(); ++i)
        {
            if(a.scores()[i] == b.scores()[i])
            {
                continue;
            }
            return a.scores()[i] < b.scores()[i];
        }
        return false;
    });
    std::uint32_t sum{0};
    for(auto i=0; i<hands.size(); ++i)
    {
        sum += (i+1)*hands[i].bet();
    }
    std::cout << sum << "\n";
}

void part1()
{
    JOKERS_WILD = false;
    play();
}

void part2()
{
    JOKERS_WILD = true;
    play();
}

int main(int argc, char* argv[])
{
    std::cout << "---- Part1 ----\n";
    part1();
    std::cout << "---- Part2 ----\n";
    part2();
}