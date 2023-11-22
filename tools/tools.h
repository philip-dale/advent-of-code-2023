#include <cstdint>
#include <iostream>
#include <ios>
#include <sstream>
#include <string>
#include <vector>

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
    std::stringstream convert(str);
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
std::vector<T> str_to_vec_sep(std::string const& data, std::string const & sub_sep, std::string const& separator=NEW_LINE)
{
    auto ret = std::vector<T>{};
    std::size_t last_pos = 0;
    auto pos = data.find(separator, last_pos);
    while(pos != std::string::npos)
    {
        ret.emplace_back(string_to<T>(data.substr(last_pos, pos-last_pos), sub_sep));
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
std::vector<T> str_to_vec(std::string const& data, std::string const& separator=NEW_LINE)
{
    auto ret = std::vector<T>{};
    std::size_t last_pos = 0;
    auto pos = data.find(separator, last_pos);
    while(pos != std::string::npos)
    {
        ret.emplace_back(string_to<T>(data.substr(last_pos, pos-last_pos)));
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
    auto ret = std::vector<T>{};
    auto data = file_to_string(filename);
    return str_to_vec_sep<T>(data, sub_sep, separator);
}

template<class T>
std::vector<T> file_to_vec(std::string const& filename, std::string const& separator=NEW_LINE)
{
    auto ret = std::vector<T>{};
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
class point {
public:
    point(T x, T y, std::string const& sep = ",") :
        m_x{x},
        m_y{y},
        m_sep{sep}
    {};

    point(std::string const& sep = ",") :
        m_x{},
        m_y{},
        m_sep{sep}
    {};

    void operator +=(point<T> val)
    {
        m_x += val.m_x;
        m_y += val.m_y;
    };

    point<T> operator +(point<T> val)
    {
        return point<T>(m_x + val.m_x, m_y + val.m_y);
    };

    friend std::istream &operator>> ( std::istream  &input, point<T> &p ) {
        std::string val; 
        std::getline(input, val);
        auto vals = str_to_vec<T>(val, p.m_sep);
        // TODO check we have 2 values
        p = point(vals[0], vals[1]); 
        return input;           
    };

    friend std::ostream &operator<< ( std::ostream &output, const point<T> &p ) { 
        output << "X : " << p.m_x << ", Y : " << p.m_y;
        return output;            
    };

    T m_x;
    T m_y;

private:
    std::string m_sep;
};

template<class T>
class pair {
public:
    pair(std::string const & sep=",") :
        a{},
        b{},
        m_sep{sep}
    {};

    pair(T a_in, T b_in, std::string const & sep=",") :
        a{a_in},
        b{b_in},
        m_sep{sep}
    {};

    friend std::ostream &operator<< ( std::ostream &output, const pair<T> &l ) { 
        output << "A [" << l.a << "], B [" << l.b << "]";
        return output;            
    };

    friend std::istream &operator>> ( std::istream  &input, pair<T> &p ) {
        std::string val;
        std::getline(input, val);
        auto vals = str_to_vec<T>(val, p.m_sep);
        // TODO check we have 2 values
        p = pair(vals[0], vals[1]); 
        return input;           
    };

    T a;
    T b;

private:
    std::string m_sep;
};

template<class T>
using line = pair<point<T>>;