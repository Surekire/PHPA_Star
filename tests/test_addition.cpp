#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"


int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }

TEST_CASE("Addition") {
    CHECK(add(2, 3) == 5);
}

TEST_CASE("Subtraction") {
    CHECK(sub(3,1) == 2);
}