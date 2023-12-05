#include "../tools/tools.h"
#include "../tools/types.h"

#include <cstdint>
#include <iostream>
#include <map>
#include <limits>

class range_lookup {
public:
    range_lookup(std::uint32_t start, std::uint32_t length, std::uint32_t result_start) :
        m_start{start},
        m_length{length},
        m_result_start{result_start}
    {};

    bool in_range(std::uint32_t v)
    {
        return v >= m_start && v < m_start + m_length;
    };

    std::uint32_t get_mapping(std::uint32_t v)
    {
        return m_result_start + (v - m_start);
    };

private:
    std::uint32_t m_start;
    std::uint32_t m_length;
    std::uint32_t m_result_start;
};

class range_map {
public:
    range_map() :
        m_ranges{}
    {};

    void append(range_lookup const & l)
    {
        m_ranges.emplace_back(l);
    };

    std::uint32_t find(std::uint32_t v)
    {
        for(auto &&range : m_ranges)
        {
            if(range.in_range(v))
            {
                return range.get_mapping(v);
            }
        }
        return v;
    };

private:
    std::vector<range_lookup> m_ranges;
};

range_map create_lookup(std::string & input)
{
    auto lines = str_to_vec<std::string>(input);
    range_map ret{};
    for(auto &&line : lines)
    {
        auto vals = str_to_vec<std::uint32_t>(line, " ");
        ret.append(range_lookup(vals[1], vals[2], vals[0]));
    }
    return ret;
}

std::uint32_t getval(range_map & m, std::uint32_t v) {
    return m.find(v);
}

class seeds_game {
public:
    seeds_game(std::string const & input) :
        m_cache{}
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
        auto s_it = m_cache.find(seed);
        if(s_it == m_cache.end())
        {
            auto v = seed;
            v = getval(m_seed_to_soil, v);
            v = getval(m_soil_to_fertilizer, v);
            v = getval(m_fertilizer_to_water, v);
            v = getval(m_water_to_light, v);
            v = getval(m_light_to_temperature, v);
            v = getval(m_temperature_to_humidity, v);
            v = getval(m_humidity_to_location, v);
            m_cache[seed] = v;
            return v;
        }
        else
        {
            return s_it->second;
        }
    }

    std::vector<std::uint32_t> m_seeds;
    range_map m_seed_to_soil;
    range_map m_soil_to_fertilizer;
    range_map m_fertilizer_to_water;
    range_map m_water_to_light;
    range_map m_light_to_temperature;
    range_map m_temperature_to_humidity;
    range_map m_humidity_to_location;

    std::map<std::uint32_t, std::uint32_t> m_cache;
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
    auto s = seeds_game(file_to_string("input_actual"));
    std::uint32_t result{std::numeric_limits<std::uint32_t>::max()};
    for(auto i=0; i< s.m_seeds.size(); i+=2)
    {
        for(auto j=s.m_seeds[0]; j<s.m_seeds[0] + s.m_seeds[1]; ++j)
        {
            std::uint32_t loc = s.get_seed_location(j);
            if(loc < result)
            {
                result = loc;
            }
        }
    }
    std::cout << result << "\n";
}

int main(int argc, char* argv[])
{
    std::cout << "---- Part1 ----\n";
    part1();
    std::cout << "---- Part2 ----\n";
    part2();
}