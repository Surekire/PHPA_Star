#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "cli.hpp"

TEST_CASE("set Flag") {
    CLI_t cli;

    cli.set_flag("test", true);
    CHECK(cli.flags.size() == 1);
    CHECK(cli.flags[0].first == "test");
    CHECK(cli.flags[0].second == true);
}

TEST_CASE("get Flag") {
    CLI_t cli;

    cli.flags.push_back({"test",true});

    CHECK(cli.get_flag("test") == true);
}

TEST_CASE("set option") {
    CLI_t cli;

    cli.set_option("test", "t");
    CHECK(cli.options.size() == 1);
    CHECK(cli.options[0].first == "test");
    CHECK(cli.options[0].second == "t");
}

TEST_CASE("get option") {
    CLI_t cli;

    cli.options.push_back({"test","t"});

    CHECK(cli.get_option("test") == "t");
}

TEST_CASE("parse Arguments") {
    CLI_t cli;

    const char* argv[] = { "test.cpp", "-t", "-p", "hier/her" };

    cli.parse_args(4, argv);

    CHECK(cli.get_flag("-t") == true);
    CHECK(cli.get_option("-p") == "hier/her");
}