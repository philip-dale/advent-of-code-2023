#include "../tools/tools.h"
#include "../tools/types.h"

#include <cstdint>
#include <iostream>
#include <optional>
#include <algorithm>

typedef std::pair<std::int64_t, std::int64_t> Position;

enum class FromDir
{
    RIGHT = 0,
    LEFT,
    TOP,
    BOTTOM,
};

FromDir get_from_dir(Position const& d)
{
    auto ret = FromDir::RIGHT;
    if(d.first == 1 && d.second == 0)
    {
        ret = FromDir::RIGHT;
    }
    if(d.first == -1 && d.second == 0)
    {
        ret = FromDir::LEFT;
    }
    if(d.first == 0 && d.second == 1)
    {
        ret = FromDir::TOP;
    }
    if(d.first == 0 && d.second == -1)
    {
        ret = FromDir::BOTTOM;
    }
    return ret;
}

class Beam {
public:
    Beam(Position const& p, Position const& d) :
        m_pos{p},
        m_direction{d}
    {};

    void move()
    {
        m_pos.first += m_direction.first;
        m_pos.second += m_direction.second;
    }

    Position const& pos()
    {
        return m_pos;
    }

    FromDir from_dir()
    {
        return get_from_dir(m_direction);
    }

    bool in_range(std::int64_t x, std::int64_t y)
    {
        return m_pos.first >= 0 && 
               m_pos.first < x && 
               m_pos.second >= 0 && 
               m_pos.second < y;
    }

    std::optional<Position> update_direction(char c)
    {
        auto from = get_from_dir(m_direction);
        switch(c)
        {
            case '.':
                return{};
            case '\\':
                if(from == FromDir::RIGHT)
                {
                    m_direction = Position{0, 1};
                }
                if(from == FromDir::LEFT)
                {
                    m_direction = Position{0, -1};
                }
                if(from == FromDir::BOTTOM)
                {
                    m_direction = Position{-1, 0};
                }
                if(from == FromDir::TOP)
                {
                    m_direction = Position{1, 0};
                }
                return{};
            case '/':
                if(from == FromDir::RIGHT)
                {
                    m_direction = Position{0, -1};
                }
                if(from == FromDir::LEFT)
                {
                    m_direction = Position{0, 1};
                }
                if(from == FromDir::BOTTOM)
                {
                    m_direction = Position{1, 0};
                }
                if(from == FromDir::TOP)
                {
                    m_direction = Position{-1, 0};
                }
                return{};
            case '|':
                if(from == FromDir::RIGHT || from == FromDir::LEFT)
                {
                    m_direction = Position{0, 1};
                    return Position{0, -1};
                }
                return {};
            case '-':
                if(from == FromDir::TOP || from == FromDir::BOTTOM)
                {
                    m_direction = Position{1, 0};
                    return Position{-1, 0};
                }
                return {};
        }
        return{};
    }

private:
    Position m_pos;
    Position m_direction;
};

typedef std::vector<FromDir> DirVec;
typedef std::vector<DirVec> DirLine;
typedef std::vector<DirLine> DirMap;

class Board {
public:
    Board(std::vector<std::string> & l, Beam b)   
    {
        layout = l;
        directions = DirMap(layout.size(), DirLine(layout[0].size(), DirVec{}));
        beams = {b};
    };

    void run()
    {
        bool change = true;
        std::vector<Beam> new_beams;
        while(change)
        {
            change = false;
            
            for(std::int64_t b=beams.size()-1; b>=0; --b)
            {
                beams[b].move();
                if(!beams[b].in_range(layout[0].size(), layout.size()) ||
                   !add_direction(beams[b].from_dir() ,beams[b].pos().first, beams[b].pos().second))
                {
                    beams.erase(beams.begin()+b);
                    continue;
                }
                change = true;
                // update direction
                auto new_beam_dir = beams[b].update_direction(layout[beams[b].pos().second][beams[b].pos().first]);
                if(new_beam_dir.has_value())
                {
                    new_beams.emplace_back(Beam(beams[b].pos(), std::move(new_beam_dir.value())));
                }
            }
            for(auto b=0; b<new_beams.size(); ++b)
            {
                beams.emplace_back(std::move(new_beams[b]));
            }
            new_beams.clear();
        }
    }

    std::int64_t energized_count()
    {
        auto sum = std::int64_t{0};
        for(auto && r : directions)
        {
            for(auto && v: r)
            {
                if(v.size() > 0)
                {
                    sum++;
                }
            }
        }
        return sum;
    }

private:
    bool add_direction(FromDir d, std::int64_t x, std::int64_t y)
    {
        auto pos = std::find(directions[y][x].begin(), directions[y][x].end(), d);
        {
            if(pos == directions[y][x].end())
            {
                directions[y][x].emplace_back(d);
                return true;
            }
        }
        return false;
    }

    std::vector<std::string> layout;
    DirMap directions;
    std::vector<Beam> beams;
};

void part1()
{
    auto layout = file_to_vec<std::string>("input_actual");
    auto board = Board(layout, Beam({-1, 0}, {1,0}));
    board.run();
    std::cout << board.energized_count() << "\n";
}

void part2()
{
    auto layout = file_to_vec<std::string>("input_actual");
    auto counts = std::vector<std::int64_t>{};
    for(auto y=0; y<layout.size(); ++y)
    {
        auto board1 = Board(layout, Beam({-1, y}, {1,0}));
        board1.run();
        counts.emplace_back(board1.energized_count());

        auto board2 = Board(layout, Beam({layout[0].size(), y}, {-1,0}));
        board2.run();
        counts.emplace_back(board2.energized_count());
    }

    for(auto x=0; x<layout[0].size(); ++x)
    {
        auto board3 = Board(layout, Beam({x, -1}, {0,1}));
        board3.run();
        counts.emplace_back(board3.energized_count());

        auto board4 = Board(layout, Beam({x, layout.size()}, {0,-1}));
        board4.run();
        counts.emplace_back(board4.energized_count());
    }

    auto val = counts[0];
    for(auto && c : counts)
    {
        if(c > val)
        {
            val = c;
        }
    }
    std::cout << val << "\n";
}

int main(int argc, char* argv[])
{
    std::cout << "---- Part1 ----\n";
    part1();
    std::cout << "---- Part2 ----\n";
    part2();
}