#include "../tools/tools.h"
#include "../tools/types.h"

#include <cstdint>
#include <iostream>
#include <map>

enum class ModuleType{
    broadcaster = 0,
    flipflop,
    conjunction,
    none
};

struct Pulse{
    std::string source;
    std::string destination;
    bool high;
};

std::string get_module_name(std::string const& v)
{
    return v[0] == 'b'? v : v.substr(1);
}

class Module {
public:
    Module() :
        name{},
        outputs{},
        output_high{false},
        inputs{},
        type{ModuleType::none},
        input_mem{}
    {};

    Module(std::string const &name_type) :
        outputs{},
        output_high{false},
        inputs{},
        input_mem{}
    {
        name = get_module_name(name_type);
        if(name_type[0] == 'b')
        {
            type = ModuleType::broadcaster;
            return;
        }
        type = name_type[0] == '%' ? ModuleType::flipflop : ModuleType::conjunction;
    }

    void add_input(std::string const& v)
    {
        inputs.emplace_back(v);
        input_mem[v] = false;
    }

    void add_output(std::string const& v)
    {
        outputs.emplace_back(v);
    }

    bool process_pulse(Pulse pulse)
    {
        if(type == ModuleType::broadcaster)
        {
            output_high = pulse.high;
            return true;
        }

        if(type == ModuleType::flipflop)
        {
            if(!pulse.high)
            {
                output_high = !output_high;
                return true;
            }
            return false;
        }

        if(type == ModuleType::conjunction)
        {
            input_mem[pulse.source] = pulse.high;
            output_high = true;
            for(auto &&m : input_mem)
            {
                output_high &= m.second;
            }
            output_high = !output_high;
            return true;
        }
        return false;
    }

    std::string name;
    std::vector<std::string> outputs;
    bool output_high;
    std::vector<std::string> inputs;
    ModuleType type;
    std::map<std::string, bool> input_mem;
};

std::map<std::string, Module> build_map(std::string const& filename)
{
    auto lines = file_to_vec<std::string>(filename);
    auto modules = std::map<std::string, Module>{};
    // create modules
    for(auto &&l : lines)
    {
        auto parts = str_to_vec<std::string>(l, " -> ");
        auto mod = Module(parts[0]);
        modules[mod.name] = std::move(mod);
    }

    //add inputs/outputs
    for(auto &&l : lines)
    {
        auto parts = str_to_vec<std::string>(l, " -> ");
        auto name = get_module_name(parts[0]);
        auto outputs = str_to_vec<std::string>(parts[1], ", ");
        for(auto &&o : outputs)
        {
            modules[name].add_output(o);
            modules[o].add_input(name);
        }
    }
    return modules;
}



void part1()
{
    auto modules = build_map("input_actual");
    auto low_pulses = std::uint64_t{0};
    auto high_pulses = std::uint64_t{0};

    for(auto i=0; i < 1000; i++){
        auto pulses = std::vector<Pulse>{{"", "broadcaster", false}};
        while(pulses.size() > 0)
        {
            auto pulse = std::move(pulses[0]);
            pulses.erase(pulses.begin());
            pulse.high ? high_pulses++ : low_pulses++;

            if(modules[pulse.destination].process_pulse(pulse))
            {
                for(auto &&o : modules[pulse.destination].outputs)
                {
                    pulses.emplace_back(Pulse{pulse.destination, o, modules[pulse.destination].output_high});
                }
            }
        }
    }
    std::cout << high_pulses * low_pulses << "\n";
}

void part2()
{
    auto modules = build_map("input_actual");
    auto button_presses = std::uint64_t{0};
    bool running =true;

    // &vd -> rx
    // &pr -> vd, &fv -> vd, &bt -> vd, &rd -> vd
    auto prev = std::map<std::string, std::uint64_t>{
        {"pr", 0},
        {"fv", 0},
        {"bt", 0},
        {"rd", 0}
    };

    while(running){
        auto pulses = std::vector<Pulse>{{"", "broadcaster", false}};
        button_presses++;
        while(pulses.size() > 0 && running)
        {
            auto pulse = std::move(pulses[0]);
            pulses.erase(pulses.begin());
            if(modules[pulse.destination].process_pulse(pulse))
            {
                for(auto &&o : modules[pulse.destination].outputs)
                {
                    if(prev.contains(pulse.destination) && modules[pulse.destination].output_high && prev[pulse.destination] == 0)
                    {
                        prev[pulse.destination] = button_presses;
                    }
                    if(prev["pr"] != 0 && prev["fv"] != 0 && prev["bt"] != 0 && prev["rd"] != 0)
                    {
                        running = false;
                        break;
                    }
                    pulses.emplace_back(Pulse{pulse.destination, o, modules[pulse.destination].output_high});
                }
            }
        }
    }
    std::cout << lcm<std::uint64_t>(std::vector<std::uint64_t>{prev["pr"], prev["fv"], prev["bt"], prev["rd"]}) << "\n";
}

int main(int argc, char* argv[])
{
    std::cout << "---- Part1 ----\n";
    part1();
    std::cout << "---- Part2 ----\n";
    part2();
}