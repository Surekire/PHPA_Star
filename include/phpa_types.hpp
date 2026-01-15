#pragma once

#include <cstddef>
#include <cstdint>
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


template<std::size_t pos_dim = 2, typename pos_type = int, typename indexType = uint32_t>
struct CSRLevel {
    // --- CSR-Struktur ---
    std::vector<indexType> row_ptr;   // size = num_nodes + 1
    std::vector<indexType> col_idx;   // size = num_edges
    std::vector<float>    weight;    // optional, size = num_edges

    // --- Knotendaten (SoA) ---
    std::vector<Position<pos_dim,pos_type>> pos;        // size = num_nodes
    std::vector<uint16_t>      level;      // size = num_nodes
    std::vector<uint32_t>      clusterID;  // size = num_nodes

    // --- Metadaten ---
    uint64_t node_count = 0;
    uint64_t edge_count = 0;
    uint16_t hierachie_level;
    const std::size_t position_dimesion = pos_dim;

    struct {
        bool error = false;
        bool clusterd = false;
    } flags;



    CSRLevel() = default;
    CSRLevel(int num_nodes) {
        row_ptr.resize(num_nodes + 1);
        clusterID.resize(num_nodes);
        node_count = num_nodes;
    }
    CSRLevel(std::vector<indexType>& col_idx, std::vector<indexType>& row_ptr) {
        node_count = row_ptr.size();
        this->row_ptr = row_ptr;
        this->col_idx = col_idx;

        clusterID.resize(node_count);
    }
    CSRLevel(std::vector<int>& col_idx, std::vector<int>& row_ptr) {
        node_count = row_ptr.size();
        this->row_ptr = row_ptr;
        this->col_idx = col_idx;

        clusterID.resize(node_count);
    }
    

};


//Point Definition, default is N = 2 and Type = int
template<std::size_t N = 2, typename T = int>
struct Hierachie {
    std::vector<CSRLevel<N,T>> levels;
    uint64_t node_count;
    uint64_t edge_count;
};
