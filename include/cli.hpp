#pragma once

/*********************************
 * * * * * * * * * * * * * * * * * 
 * Command Line Interface Header *
 * * * * * * * * * * * * * * * * *
 *********************************/


//Includes

#include <string>
#include <vector>
#include <utility>



class CLI_t {
public:

    std::vector<std::pair<std::string, bool>> flags;
    std::vector<std::pair<std::string, std::string>> options;


    CLI_t(/* args */);
    ~CLI_t();

    void parse_args(int argc, const char** argv);

    void set_flag(std::string flag, bool value);
    bool get_flag(std::string flag);

    void set_option(std::string option, std::string value);
    std::string get_option(std::string option);
};
