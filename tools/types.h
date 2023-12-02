#pragma once
#include "tools.h"

#include <iostream>
#include <regex>
#include <streambuf>
#include <vector>

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

class num_str{
public:
    num_str() :
        num{0},
        str{""}
    {};

    friend std::ostream &operator<< ( std::ostream &output, const num_str &n ) { 
        output << "num = " << n.num << ", str = " << n.str ;
        return output;            
    };

    friend std::istream &operator>> ( std::istream  &input, num_str &n ) {
        std::string val;
        std::getline(input, val);
        std::regex split(" ?(\\d+) (.+)");
        std::smatch match;
        std::regex_search(val, match, split);
        n.num = stoi(match[1].str());
        n.str = match[2].str();
        return input;           
    };

    std::uint64_t get_num()
    {
        return num;
    };

    std::string get_str()
    {
        return str;
    };

protected:
    std::uint64_t num;
    std::string str;
};

class str_num : public num_str {
public:
    str_num() :
        num_str()
    {};

    friend std::istream &operator>> ( std::istream  &input, str_num &n ) {
        std::string val;
        std::getline(input, val);
        std::regex split(" ?(.+) (\\d+)");
        std::smatch match;
        std::regex_search(val, match, split);
        n.num = stoi(match[2].str());
        n.str = match[1].str();
        return input;           
    };
};

template<class T>
using line = pair<point<T>>;