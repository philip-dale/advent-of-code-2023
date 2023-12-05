#include "../tools/tools.h"
#include "../tools/types.h"

#include <cstdint>
#include <iostream>
#include <map>
#include <limits>

typedef std::map<std::uint32_t, std::uint32_t> seed_map;

seed_map create_lookup(std::string & input)
{
    auto lines = str_to_vec<std::string>(input);
    seed_map ret{};
    for(auto &&line : lines)
    {
        auto vals = str_to_vec<std::uint32_t>(line, " ");
        for (auto i=0; i<vals[2]; ++i)
        {
            ret[vals[1]+i] = vals[0] + i;
        }
    }
    return ret;
}

std::uint32_t getval(seed_map & m, std::uint32_t v) {
    auto m_it = m.find(v);
    if(m_it == m.end())
    {
        return v;
    }
    return m_it->second;
}

class seeds_game {
public:
    seeds_game(std::string const & input)
    {
        auto sections = str_to_vec<std::string>(input, std::string(NEW_LINE) + std::string(NEW_LINE));
        for( auto &&section : sections)
        {
            auto fields = str_to_vec<std::string>(section, ":");
            if(fields[0] == "seeds")
            {
                m_seeds = str_to_vec<std::uint32_t>(fields[1], " ");
                continue;
            }
            if(fields[0] == "seed-to-soil map")
            {
                m_seed_to_soil = create_lookup(fields[1]);
                continue;
            }
            if(fields[0] == "soil-to-fertilizer map")
            {
                m_soil_to_fertilizer = create_lookup(fields[1]);
                continue;
            }
            if(fields[0] == "fertilizer-to-water map")
            {
                m_fertilizer_to_water = create_lookup(fields[1]);
                continue;
            }
            if(fields[0] == "water-to-light map")
            {
                m_water_to_light = create_lookup(fields[1]);
                continue;
            }
            if(fields[0] == "light-to-temperature map")
            {
                m_light_to_temperature = create_lookup(fields[1]);
                continue;
            }
            if(fields[0] == "temperature-to-humidity map")
            {
                m_temperature_to_humidity = create_lookup(fields[1]);
                continue;
            }
            if(fields[0] == "humidity-to-location map")
            {
                m_humidity_to_location = create_lookup(fields[1]);
                continue;
            }
        }
    };

    std::uint32_t get_seed_location(std::uint32_t seed)
    {
        auto v = seed;
        v = getval(m_seed_to_soil, v);
        v = getval(m_soil_to_fertilizer, v);
        v = getval(m_fertilizer_to_water, v);
        v = getval(m_water_to_light, v);
        v = getval(m_light_to_temperature, v);
        v = getval(m_temperature_to_humidity, v);
        v = getval(m_humidity_to_location, v);
        return v;
    }

    std::vector<std::uint32_t> m_seeds;
    seed_map m_seed_to_soil;
    seed_map m_soil_to_fertilizer;
    seed_map m_fertilizer_to_water;
    seed_map m_water_to_light;
    seed_map m_light_to_temperature;
    seed_map m_temperature_to_humidity;
    seed_map m_humidity_to_location;
};

void part1()
{
    auto s = seeds_game(file_to_string("input_actual"));
    std::uint32_t result{std::numeric_limits<std::uint32_t>::max()};
    for(auto &&seed : s.m_seeds)
    {
        std::uint32_t loc = s.get_seed_location(seed);
        if(loc < result)
        {
            result = loc;
        }
    }
    std::cout << result << "\n";
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