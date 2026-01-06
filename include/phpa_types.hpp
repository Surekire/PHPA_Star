#pragma once

#include <cstddef>
#include <array>
#include <vector>

///////////////////////
//Main Types of phpa*//
///////////////////////

//Point Definition, default is N = 2 and Type = int
//Size: N * 4 Bytes
template<std::size_t N = 2, typename T = int>
struct Position {
    static_assert(N > 0, "Position dimension must be > 0");

    std::array<T, N> v;

    T& operator[](std::size_t i) { return v[i]; }
    const T& operator[](std::size_t i) const { return v[i]; }
};

template<typename T>
struct Position<2, T> {
    union {
        struct { T x, y; };
        std::array<T, 2> v;
    };

    T& operator[](std::size_t i) { return v[i]; }
    const T& operator[](std::size_t i) const { return v[i]; }
};

template<typename T>
struct Position<3, T> {
    union {
        struct { T x, y, z; };
        std::array<T, 3> v;
    };

    T& operator[](std::size_t i) { return v[i]; }
    const T& operator[](std::size_t i) const { return v[i]; }
};



//Point Definition, default is N = 2 and Type = int
//Size: N * 4 + 8 Bytes -> Default 16 Bytes
template<std::size_t N = 2, typename T = int>
struct Node {
    std::uint16_t degree;
    std::uint16_t level;
    std::uint32_t clusterID;
    Position<N,T> pos;
};

//Point Definition, default is N = 2 and Type = int
//
//graph is an Adjazentcy List 
template<std::size_t N = 2, typename T = int>
struct Level {
    std::vector<std::vector<Node<N,T>>> graph;
    
};

//Point Definition, default is N = 2 and Type = int
template<std::size_t N = 2, typename T = int>
struct Hierachie {
    std::vector<Level<N,T>>;
};