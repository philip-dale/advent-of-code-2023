#include <cstdint>
#include <sstream>
#include <string>
#include <vector>


std::string to_string(std::vector<std::uint8_t> const & data);
std::string to_hex_string(std::vector<std::uint8_t> const & data);
std::string file_to_string(std::string const& filename);

template<class T>
std::vector<T> file_to_vec(std::string const& filename, std::string const& separator="\r\n")
{
    auto ret = std::vector<T>{};
    auto data = file_to_string(filename);
    std::size_t last_pos = 0;
    auto pos = data.find(separator, last_pos);
    while(pos != std::string::npos)
    {
        std::stringstream convert(data.substr(last_pos, pos-last_pos));
        T value;
        convert >> value;
        ret.emplace_back(value);
        last_pos = pos + separator.size();
        pos = data.find(separator, last_pos);
    }
    return ret;
}

template<class T>
void print_vec(std::vector<T> const& data)
{
    for(auto &&str : data)
    {
        std::cout << str << "\n";
    }
}