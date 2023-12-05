#include "../tools/tools.h"
#include "../tools/types.h"

#include <cstdint>
#include <iostream>
#include <map>
#include <limits>

class range {
public:
    range(std::uint64_t start, std::uint64_t end) :
        m_start{start},
        m_end{end}
    {};

    std::uint64_t start(){
        return m_start;
    };

    std::uint64_t end(){
        return m_end;
    };

    bool valid()
    {
        return m_start <= m_end;
    }

    bool in_range(std::uint64_t v)
    {
        return v >= m_start && v < m_end;
    };

    bool in_range(range & r)
    {
        return r.start() >= m_start && r.end() <= m_end;
    }

    std::vector<range> spit_range(std::vector<range> ranges)
    {
        auto ret = std::vector<range>{};
        for(auto && r : ranges)
        {
            // 3 posible outputs, left, mid, right
            auto left = range(r.start(), std::min(r.end(), m_end));
            if(left.valid())
            {
                ret.emplace_back(left);
            }
            auto mid = range(std::max(r.start(), m_end), std::min(r.end(), m_end));
            if(mid.valid())
            {
                ret.emplace_back(mid);
            }
            auto right = range(std::max(r.start(), m_end), r.end());
            if(right.valid())
            {
                ret.emplace_back(right);
            }
        }

        return ret;
    };
protected:
    std::uint64_t m_start;
    std::uint64_t m_end;
};

class range_lookup : public range {
public:
    range_lookup(std::uint64_t start, std::uint64_t length, std::uint64_t result_start) :
        range(start, start+length),
        m_result_start{result_start}
    {};

    void apply_mapping(std::uint64_t & v)
    {
        v = m_result_start + (v - m_start);
    };

    void apply_mapping(range & r)
    {
        r = range(m_result_start + (r.start() - m_start), m_result_start + (r.end() - m_start));
    };

private:
    std::uint64_t m_result_start;
};

class range_lookup_group {
public:
    range_lookup_group() :
        m_range_lookups{}
    {};

    range_lookup_group(std::string const & s) :
        m_range_lookups{}
    {
        auto lines = str_to_vec<std::string>(s);
        for(auto &&line : lines)
        {
            auto vals = str_to_vec<std::uint64_t>(line, " ");
            m_range_lookups.emplace_back(range_lookup(vals[1], vals[2], vals[0]));
        }
    };

    void apply_mapping(std::uint64_t & v)
    {
        for(auto &&range : m_range_lookups)
        {
            if(range.in_range(v))
            {
                range.apply_mapping(v);
                return;
            }
        }
    };

    void apply_mapping(std::vector<range> & ranges)
    {
        for(auto &&range_lookup : m_range_lookups)
        {
            ranges = range_lookup.spit_range(ranges);
        }

        // now appy mappings
        for(auto i=0; i< ranges.size(); ++i)
        {
            for(auto &&range_lookup : m_range_lookups)
            {
                if(range_lookup.in_range(ranges[i]))
                {
                    range_lookup.apply_mapping(ranges[i]);
                    break;
                }
            }
        }
    }

private:
    std::vector<range_lookup> m_range_lookups;
};

class seeds_game {
public:
    seeds_game(std::string const & input) :
        m_groups{}
    {
        auto sections = str_to_vec<std::string>(input, std::string(NEW_LINE) + std::string(NEW_LINE));
        for( auto &&section : sections)
        {
            auto fields = str_to_vec<std::string>(section, ":");
            if(fields[0] == "seeds")
            {
                m_seeds = str_to_vec<std::uint64_t>(fields[1], " ");
                continue;
            }
            else
            {
                m_groups.emplace_back(range_lookup_group(fields[1]));
            }
        }
    };

    std::vector<std::uint64_t> &seeds()
    {
        return m_seeds;
    }

    template<class T>
    void get_seed_location(T & seed)
    {
        for(auto &&g : m_groups)
        {
            g.apply_mapping(seed);
        }
    }

private:
    std::vector<std::uint64_t> m_seeds;
    std::vector<range_lookup_group> m_groups;
};

void part1()
{
    auto s = seeds_game(file_to_string("input_actual"));
    std::uint64_t result{std::numeric_limits<std::uint64_t>::max()};
    for(auto &&seed : s.seeds())
    {
        std::uint64_t loc = seed;
        s.get_seed_location(loc);
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

    auto ranges = std::vector<range>{};
    for(auto i=0; i< s.seeds().size(); i+=2)
    {
        ranges.emplace_back(range(s.seeds()[i], s.seeds()[i] + s.seeds()[i+1]));
    }

    s.get_seed_location(ranges);

    std::uint64_t result{std::numeric_limits<std::uint64_t>::max()};
    for(auto &&r : ranges)
    {
        if(r.start() < result)
        {
            result = r.start();
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