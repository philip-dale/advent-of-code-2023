#pragma once

#include <cstdint>
#include <iostream>
#include <iomanip>
#include <ios>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <numeric>
#include <cmath>

#ifdef _WIN32
    #define NEW_LINE "\r\n"
#else
    #define NEW_LINE "\n"
#endif

std::string to_string(std::vector<std::uint8_t> const & data);
std::string to_hex_string(std::vector<std::uint8_t> const & data);
std::string file_to_string(std::string const& filename);

template<class T> 
T string_to(std::string const& str)
{
    if constexpr (std::is_same_v<T, std::string>)
    {
        return str;
    }
    std::istringstream convert(str);
    T value;
    convert >> value;
    return value;
}

template<class T> 
T string_to(std::string const& str, std::string const & sep)
{
    std::stringstream convert(str);
    T value{sep};
    convert >> value;
    return value;
}

template<class T>
std::vector<T> str_to_vec_sep(std::string const& data, std::string const & sub_sep, std::string const& separator=NEW_LINE, bool keep_empty = false)
{
    auto ret = std::vector<T>{};
    std::size_t last_pos = 0;
    auto pos = data.find(separator, last_pos);
    while(pos != std::string::npos)
    {
        auto val = data.substr(last_pos, pos-last_pos);
        if(val.empty())
        {
            if(keep_empty)
            {
                ret.emplace_back(T());
            }
        }
        else
        {
            ret.emplace_back(string_to<T>(val));
        }
        last_pos = pos + separator.size();
        pos = data.find(separator, last_pos);
    }
    // Handle not having separator at end of a line
    if(last_pos < data.size())
    {
        ret.emplace_back(string_to<T>(data.substr(last_pos)));
    }
    return ret;
};

template<class T>
std::vector<T> str_to_vec(std::string const& data, std::string const& separator=NEW_LINE, bool keep_empty = false)
{
    auto ret = std::vector<T>{};
    std::size_t last_pos = 0;
    auto pos = data.find(separator, last_pos);
    while(pos != std::string::npos)
    {
        auto val = data.substr(last_pos, pos-last_pos);
        if(val.empty())
        {
            if(keep_empty)
            {
                ret.emplace_back(T());
            }
        }
        else
        {
            ret.emplace_back(string_to<T>(val));
        }
        last_pos = pos + separator.size();
        pos = data.find(separator, last_pos);
    }
    // Handle not having separator at end of a line
    if(last_pos < data.size())
    {
        ret.emplace_back(string_to<T>(data.substr(last_pos)));
    }
    return ret;
};

template<class T>
std::vector<T> file_to_vec_sep(std::string const& filename, std::string const & sub_sep, std::string const& separator=NEW_LINE)
{
    auto data = file_to_string(filename);
    return str_to_vec_sep<T>(data, sub_sep, separator);
}

template<class T>
std::vector<T> file_to_vec(std::string const& filename, std::string const& separator=NEW_LINE)
{
    auto data = file_to_string(filename);
    return str_to_vec<T>(data, separator);
}

template<class T>
void print_vec(std::vector<T> const& data)
{
    for(auto &&str : data)
    {
        std::cout << str << "\n";
    }
}

template<class T>
std::vector<std::vector<T>> str_to_2d(std::string const& data, std::string const & sub_sep, std::string const& separator=NEW_LINE)
{
    auto lines = str_to_vec<std::string>(data, separator);
    auto ret = std::vector<std::vector<T>>{};
    for(auto &&l : lines)
    {
        ret.emplace_back(str_to_vec<T>(l, sub_sep));
    }
    return ret;
};

template<class T>
std::vector<std::vector<T>> file_to_2d(std::string const& filename, std::string const & sub_sep, std::string const& separator=NEW_LINE)
{
    auto data = file_to_string(filename);
    return str_to_2d<T>(data, sub_sep, separator);
}

template<class T>
void print_2d(std::vector<std::vector<T>> const& data, std::size_t width=3, char pad=' ')
{
    for(auto &&x : data)
    {
        for(auto &&y : x)
        {
            std::cout << std::setfill(pad) << std::setw(width) << y << ", ";
        }
        std::cout << "\n";
    }
}

struct area {
    std::size_t row_start;
    std::size_t col_start;
    std::size_t row_end;
    std::size_t col_end;

    std::size_t row_size()
    {
        return row_end - row_start + 1;
    };
    std::size_t col_size()
    {
        return col_end - col_start + 1;
    };
    std::size_t area_size()
    {
        return row_size() * col_size();
    };
};

template<class T>
area get_bounding_box(std::vector<T> const & input_vec, area const & input_area, std::size_t width)
{
    auto ret = area{0,0,0,0};
    auto col_size = input_vec[0].size();
    auto row_size = input_vec.size();

    ret.col_start = input_area.col_start >= width ? input_area.col_start - width : 0;
    ret.col_end = input_area.col_end <= col_size - 1 - width ? input_area.col_end + width : col_size -1;
    ret.row_start = input_area.row_start >= width ? input_area.row_start - width : 0;
    ret.row_end = input_area.row_end <= row_size - 1 - width ? input_area.row_end + width : row_size -1;

    return ret;
};

std::vector<double> solve_quad(double a, double b, double c);
std::string remove_space(std::string & in);

class node {
public:
    node() :
        m_id{0},
        m_neighbours{},
        m_dist{std::numeric_limits<std::uint32_t>::max()},
        m_previous{0},
        m_weight{0},
        m_processed{false}
    {};

    node(std::size_t id, std::vector<std::size_t> & neighbours, std::uint32_t weight) :
        m_id{id},
        m_neighbours{neighbours},
        m_dist{std::numeric_limits<std::uint32_t>::max()},
        m_previous{},
        m_weight{weight},
        m_processed{false}
    {};

    bool update_dist(std::uint32_t v)
    {
        if(v < m_dist)
        {
            m_dist = v;
            return true;
        }
        return false;
    };

    void set_previous(std::size_t p)
    {
        m_previous = p;
        m_processed = true;
    };

    std::size_t get_previous()
    {
        return m_previous;
    };

    std::size_t id()
    {
        return m_id;
    };

    std::vector<std::size_t> neighbours()
    {
        return m_neighbours;
    };

    void add_neighbour(std::size_t n)
    {
        m_neighbours.emplace_back(n);
    }

    bool processed()
    {
        return m_processed;
    };

    std::uint32_t dist()
    {
        return m_dist;
    };

    std::uint32_t weight()
    {
        return m_weight;
    };

private:
    std::size_t m_id;
    std::vector<std::size_t> m_neighbours;
    std::uint32_t m_dist;
    std::size_t m_previous;
    std::uint32_t m_weight;
    bool m_processed;
};

template<class T>
std::vector<std::pair<std::size_t, std::size_t>> aLL_neighbours_2d(std::vector<std::vector<T>> & data, std::size_t x, std::size_t y)
{
    auto neighbours = std::vector<std::pair<std::size_t, std::size_t>>{};
    if(x != 0)
    {
        neighbours.emplace_back(std::pair<std::size_t, std::size_t>(x-1, y));
        if(y != 0)
        {
            neighbours.emplace_back(std::pair<std::size_t, std::size_t>(x-1, y-1));
        }
        if(y != data.size())
        {
            neighbours.emplace_back(std::pair<std::size_t, std::size_t>(x-1, y+1));
        } 
    }
    if(x != data[0].size())
    {
        neighbours.emplace_back(std::pair<std::size_t, std::size_t>(x+1, y));
        if(y != 0)
        {
            neighbours.emplace_back(std::pair<std::size_t, std::size_t>(x+1, y-1));
        }
        if(y != data.size())
        {
            neighbours.emplace_back(std::pair<std::size_t, std::size_t>(x+1, y+1));
        } 
    }
    if(y != 0)
    {
        neighbours.emplace_back(std::pair<std::size_t, std::size_t>(x, y-1));
        if(x != 0)
        {
            neighbours.emplace_back(std::pair<std::size_t, std::size_t>(x-1, y-1));
        }
        if(x != data[0].size())
        {
            neighbours.emplace_back(std::pair<std::size_t, std::size_t>(x+1, y-1));
        }
    }
    if(y != data.size())
    {
        neighbours.emplace_back(std::pair<std::size_t, std::size_t>(x, y+1));
        if(x != 0)
        {
            neighbours.emplace_back(std::pair<std::size_t, std::size_t>(x-1, y+1));
        }
        if(x != data[0].size())
        {
            neighbours.emplace_back(std::pair<std::size_t, std::size_t>(x+1, y+1));
        }
    } 
    return neighbours;
}

template<class T>
std::vector<std::pair<std::size_t, std::size_t>> adjacent_neighbours_2d(std::vector<std::vector<T>> & data, std::size_t x, std::size_t y)
{
    auto neighbours = std::vector<std::pair<std::size_t, std::size_t>>{};
    if(x != 0)              neighbours.emplace_back(std::pair<std::size_t, std::size_t>(x-1, y));
    if(x != data[0].size()-1) neighbours.emplace_back(std::pair<std::size_t, std::size_t>(x+1, y));
    if(y != 0)              neighbours.emplace_back(std::pair<std::size_t, std::size_t>(x, y-1));
    if(y != data.size()-1)    neighbours.emplace_back(std::pair<std::size_t, std::size_t>(x, y+1));  
    return neighbours;
}

template<class T>
std::vector<std::size_t> adjacent_neighbours_1d(std::vector<std::vector<T>> & data, std::size_t x, std::size_t y)
{
    auto points = adjacent_neighbours_2d(data, x, y);
    auto neighbours = std::vector<std::size_t>{};
    for(auto &&p : points)
    {
        neighbours.emplace_back(p.first +  (p.second*data[0].size()));
    }
    return neighbours;
}

template<class T>
std::vector<node> two_d_vec_to_nodes(std::vector<std::vector<T>> & data)
{
    auto nodes = std::vector<node>(data.size() * data[0].size());
    for(auto i=0; i<data.size(); ++i)
    {
        for(auto j=0; j<data.size(); ++j)
        {
            auto pos = j + (i*data[0].size());
            auto neighbours = adjacent_neighbours_1d(data, j, i);
            nodes[pos] = node(pos, neighbours, data[i][j]);
        }
    }
    return nodes;
}

template<class T>
T lcm(std::vector<T> const & v)
{
    // Use Chinese remainder theorem to get lowest common multiple
    auto lcm = T{1};
    for(auto && n : v)
    {
        lcm = std::floor((n*lcm)/std::gcd(n,lcm));
    }
    return lcm;
}

void dijkstra(std::vector<node> & nodes, std::size_t start, bool do_all = true, std::size_t end=0);
