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
    if(d.first == 1 && d.second == 0)
    {
        return FromDir::RIGHT;
    }
    if(d.first == -1 && d.second == 0)
    {
        return FromDir::LEFT;
    }
    if(d.first == 0 && d.second == 1)
    {
        return FromDir::TOP;
    }
    if(d.first == 0 && d.second == -1)
    {
        return FromDir::BOTTOM;
    }
    return FromDir::RIGHT;
}

class Beam {
public:
    Beam(Position const& p) :
        pos{p},
        direction{1,0}
    {};

    Beam(Position const& p, Position const& d) :
        pos{p},
        direction{d}
    {};

    void move()
    {
        pos.first += direction.first;
        pos.second += direction.second;
    }

    Position const& get_pos()
    {
        return pos;
    }

    FromDir get_dir()
    {
        return get_from_dir(direction);
    }

    bool in_range(std::int64_t x, std::int64_t y)
    {
        return pos.first >= 0 && pos.first < x && pos.second >= 0 && pos.second < y;
    }

    std::optional<Position> update_direction(char c)
    {
        auto from = get_from_dir(direction);
        switch(c)
        {
            case '.':
                return{};
            case '\\':
                if(from == FromDir::RIGHT)
                {
                    direction = Position{0, 1};
                }
                if(from == FromDir::LEFT)
                {
                    direction = Position{0, -1};
                }
                if(from == FromDir::BOTTOM)
                {
                    direction = Position{-1, 0};
                }
                if(from == FromDir::TOP)
                {
                    direction = Position{1, 0};
                }
                return{};
            case '/':
                if(from == FromDir::RIGHT)
                {
                    direction = Position{0, -1};
                }
                if(from == FromDir::LEFT)
                {
                    direction = Position{0, 1};
                }
                if(from == FromDir::BOTTOM)
                {
                    direction = Position{1, 0};
                }
                if(from == FromDir::TOP)
                {
                    direction = Position{-1, 0};
                }
                return{};
            case '|':
                if(from == FromDir::RIGHT || from == FromDir::LEFT)
                {
                    direction = Position{0, 1};
                    return Position{0, -1};
                }
                return {};
            case '-':
                if(from == FromDir::TOP || from == FromDir::BOTTOM)
                {
                    direction = Position{1, 0};
                    return Position{-1, 0};
                }
                return {};
                
        }
        return{};
    }

private:
    Position pos;
    Position direction;
};

typedef std::vector<std::vector<std::vector<FromDir>>> DirectionsMap;

class Board {
public:
    Board(std::string const& filename)   
    {
        layout = file_to_vec<std::string>(filename);
        counts = std::vector<std::vector<std::int64_t>>(layout.size(), std::vector<std::int64_t>(layout[0].size(),0));
        directions = DirectionsMap(
            layout.size(), 
            std::vector<std::vector<FromDir>>(layout[0].size(),std::vector<FromDir>{})
        );
        beams = {Beam({-1, 0})};
    };
    Board(std::vector<std::string> & l, Beam b)   
    {
        layout = l;
        counts = std::vector<std::vector<std::int64_t>>(layout.size(), std::vector<std::int64_t>(layout[0].size(),0));
        directions = DirectionsMap(
            layout.size(), 
            std::vector<std::vector<FromDir>>(layout[0].size(),std::vector<FromDir>{})
        );
        beams = {b};
    };

    void run()
    {
        bool change = true;
        auto old_directions = directions;
        do
        {
            old_directions = directions;
            std::vector<Beam> new_beams;
            for(std::int64_t b=beams.size()-1; b>=0; --b)
            {
                beams[b].move();
                if(beams[b].in_range(layout[0].size(), layout.size()))
                {
                    counts[beams[b].get_pos().second][beams[b].get_pos().first] ++;
                    add_direction(beams[b].get_dir() ,beams[b].get_pos().first, beams[b].get_pos().second);
                    // update direction
                    auto new_beam_dir = beams[b].update_direction(layout[beams[b].get_pos().second][beams[b].get_pos().first]);
                    if(new_beam_dir.has_value())
                    {
                        new_beams.emplace_back(Beam(beams[b].get_pos(), new_beam_dir.value()));
                    }
                }
                else
                {
                    beams.erase(beams.begin()+b);
                }
            }
            for(auto && b : new_beams)
            {
                beams.emplace_back(std::move(b));
            }
        }
        while(directions_changed(old_directions));
    }

    bool directions_changed(DirectionsMap & d)
    {
        for(auto r=0; r<directions.size(); ++r)
        {
            for(auto c=0; c<directions[0].size(); ++c)
            {
                if(directions[r][c].size() != d[r][c].size())
                {
                    return true;
                }
            }
        }
        return false;
    }

    void add_direction(FromDir d, std::int64_t x, std::int64_t y)
    {
        auto pos = std::find(directions[y][x].begin(), directions[y][x].end(), d);
        {
            if(pos == directions[y][x].end())
            {
                directions[y][x].emplace_back(d);
            }
        }
    }

    std::int64_t energized_count()
    {
        auto sum = std::int64_t{0};
        for(auto && r : counts)
        {
            for(auto && v: r)
            {
                if(v > 0)
                {
                    sum++;
                }
            }
        }
        return sum;
    }

private:
    std::vector<std::string> layout;
    std::vector<std::vector<std::int64_t>> counts;
    DirectionsMap directions;
    std::vector<Beam> beams;
};

void part1()
{
    auto board = Board("input_actual");
    board.run();
    std::cout << board.energized_count() << "\n";
}

void part2()
{
    auto layout = file_to_vec<std::string>("input_actual");
    auto counts = std::vector<std::int64_t>{};
    for(auto y=0; y<layout.size(); ++y)
    {
        auto board1 = Board(layout, Beam(Position{-1, y}, {1,0}));
        board1.run();
        counts.emplace_back(board1.energized_count());

        auto board2 = Board(layout, Beam(Position{layout[0].size(), y}, {-1,0}));
        board2.run();
        counts.emplace_back(board2.energized_count());
        std::cout << y << "\n";
    }

    for(auto x=0; x<layout[0].size(); ++x)
    {
        auto board3 = Board(layout, Beam(Position{x, -1}, {0,1}));
        board3.run();
        counts.emplace_back(board3.energized_count());

        auto board4 = Board(layout, Beam(Position{x, layout.size()}, {0,-1}));
        board4.run();
        counts.emplace_back(board4.energized_count());
        std::cout << x << "\n";
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