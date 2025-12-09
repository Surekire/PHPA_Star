#include "cli.hpp"
#include <stdexcept>

CLI_t::CLI_t() {
    flags.clear();
    options.clear();
}

CLI_t::~CLI_t() {}

void CLI_t::set_flag(std::string flag, bool value) {
    for(auto& it : flags) {
        if(it.first == flag) {
            it.second = value;
            return;
        }
    }

    //flag not Found

    flags.push_back({flag, value});
}

void CLI_t::set_option(std::string option, std::string value) {
    for(auto& it : options) {
        if(it.first == option) {
            it.second = value;
            return;
        }
    }

    //option not Found

    options.push_back({option, value});
}



bool CLI_t::get_flag(std::string flag) {
    for(auto& it : flags) {
        if(it.first == flag) {
            return it.second;
        }
    }

    //flag not found

    printf("%s: Error in get_flag() - flag not found: %s", __FILE__, flag.c_str());
    throw std::invalid_argument("flag not found: " + flag);
}

std::string CLI_t::get_option(std::string option) {
    for(auto& it : options) {
        if(it.first == option) {
            return it.second;
        }
    }

    //option not found

    printf("%s: Error in get_option() - option not found: %s", __FILE__, option.c_str());
    throw std::invalid_argument("Option not found: " + option);
}

void CLI_t::parse_args(int argc, const char** argv) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.rfind("-", 0) == 0) { // beginnt mit '-' oder '--'
            if (i + 1 < argc && std::string(argv[i + 1]).rfind("-", 0) != 0) {
                // Option mit Wert
                set_option(arg, argv[i + 1]);
                ++i; // Wert überspringen
            } else {
                // reines Flag
                set_flag(arg, true);
            }
        }
    }
}