#include "../tools/tools.h"
#include "../tools/types.h"

#include <algorithm>
#include <cstdint>
#include <iostream>

typedef std::pair<std::string, std::int64_t> lens_info;

class box{
public:
    box() :
        slots{}
    {};

    void add(lens_info const& l)
    {
        auto pos = std::find_if(slots.begin(), slots.end(), [l](lens_info const& a)
        {
            return a.first == l.first;
        });
        if(pos == slots.end())
        {
            slots.emplace_back(l);
        }
        else
        {
            pos->second = l.second;
        }
    };

    void remove(lens_info const& l)
    {
        auto pos = std::find_if(slots.begin(), slots.end(), [l](lens_info const& a)
        {
            return a.first == l.first;
        });
        if(pos != slots.end())
        {
            slots.erase(pos);
        }
    }

    std::size_t size()
    {
        return slots.size();
    };

    lens_info &operator[](std::size_t i)
    {
        return slots[i];
    };

private:
    std::vector<lens_info> slots;
};

void update_hash(std::int64_t & v, char c)
{
    v += static_cast<std::int64_t>(c);
    v *= 17;
    v = v % 256;
}

std::int64_t gen_hash(std::string const & s)
{
    auto hash = std::int64_t{0};
    for(auto &&c : s)
    {
        update_hash(hash, c);
    }
    return hash;
}

void part1()
{
    auto parts = file_to_vec<std::string>("input_actual", ",");
    auto sum = std::int64_t{0};
    for(auto &&p : parts)
    {
        sum += gen_hash(p);
    }
    std::cout << sum << "\n";
}

void part2()
{
    auto parts = file_to_vec<std::string>("input_actual", ",");
    
    auto boxes = std::vector<box>(256);
    for(auto &&p : parts)
    {
        auto symbol_pos = p.find_first_of("-=");
        auto label = p.substr(0, symbol_pos);
        auto hash = gen_hash(label);
        if(p.ends_with("-"))
        {
            boxes[hash].remove({label, 0});
        }
        else
        {
            auto lens = p[p.size()-1] - '0';
            boxes[hash].add({label, lens});
        }
    }

    auto sum = std::int64_t{0};
    for(auto b=0; b<boxes.size(); ++b)
    {
        for(auto s=0; s< boxes[b].size(); ++s)
        {
            sum += (b+1) * (s+1) * boxes[b][s].second;
        }
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