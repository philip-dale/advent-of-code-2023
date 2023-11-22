#include <cstdint>
#include <iostream>
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
    point(T x, T y) :
        m_x{x},
        m_y{y}
    {};

    point() :
        m_x{0},
        m_y{0}
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
        input >> val;
        auto vals = str_to_vec<T>(val, ",");
        // TODO check we have 2 values
        p = point(vals[0], vals[1]); 
        return input;           
    };

    friend std::ostream &operator<< ( std::ostream &output, const point<T> &p ) { 
        output << "X : " << p.m_x << ", Y : " << p.m_y;
        return output;            
    };

private:
    T m_x;
    T m_y;
};

template<class T>
class line {
public:
    line() :
        a{},
        b{}
    {};

    line(point<T> a_in, point<T> b_in) :
        a{a_in},
        b{b_in}
    {};

    friend std::ostream &operator<< ( std::ostream &output, const line<T> &l ) { 
        output << "A [ " << l.a << "], B [ " << l.b << "]";
        return output;            
    };

    point<T> a;
    point<T> b;
};