#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "phpa_types.hpp"

TEST_CASE("position add") {
    Position<3> a = {1,1,1};
    Position<3> b = {0,2,4};

    Position<3> c = a + b;

    CHECK(c.v[0] == 1);
    CHECK(c.v[1] == 3);
    CHECK(c.v[2] == 5);
}

TEST_CASE("postion subtract") {
    Position<3> a = {1,3,5};
    Position<3> b = {0,2,4};

    Position<3> c = a - b;

    CHECK(c.v[0] == 1);
    CHECK(c.v[1] == 1);
    CHECK(c.v[2] == 1);
}

TEST_CASE("postion get") {
    Position<10> a = {0,1,2,3,4,5,6,7,8,9};
    for(int i = 0; i < a.size(); ++i) {
        CHECK(a[i] == a.v[i]);
        CHECK(a[i] == i);
    }
} 