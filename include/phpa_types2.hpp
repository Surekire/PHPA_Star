#pragma once

#include <vector>
#include <array>
#include <cstdint>
#include <memory>
#include <unordered_map>

static constexpr size_t Dim = 2;
static constexpr size_t max_cluster_size = 256;



struct Path {
    std::vector<uint64_t> nodes;
    std::vector<float> distances;
    float total_distance;

    void add(uint64_t node, float dist) {
        nodes.push_back(node);
        distances.push_back(dist);
    }
};


struct Level_CSR {

    // Meta Data
    uint64_t cluster_count;
    uint64_t node_count;
    uint64_t edge_count;
    uint64_t gates;
    uint64_t level;


    // Cluster Data
    std::vector<uint64_t> cluster_ptn;
    

    // Node Data
    std::vector<uint64_t> row_ptn;
    std::vector<uint64_t> clusterID;
    std::vector<uint8_t> is_gate; //wegen Cache?
    
    std::vector<std::array<uint32_t, Dim>> pos; //[node][dim]
    
    std::unordered_map<uint64_t, uint64_t> uplink;  //Not every Node is an Gate
    std::vector<uint64_t> downlink; //Every Node is Gate of Prev level


    // Edge Data
    std::vector<uint64_t> col_index;
    std::vector<float> weight;



    // Consturctors


    Level_CSR(uint64_t cluster_count, uint64_t node_count, uint64_t edge_count) {
        cluster_ptn.resize(cluster_count + 1);
        
        row_ptn.resize(node_count + 1);
        clusterID.resize(node_count);
        is_gate.resize(node_count);
        pos.resize(node_count);

        col_index.resize(edge_count);
        weight.resize(edge_count);
    }


    //Methodes

    void add_edge();
    void add_node();
    
    //Reorders Nodes to pack there Cluster ID together
    void reorder();
    void cluster();


    //Pathfinding
    Path A_star(uint64_t start, uint64_t end); //Inter Cluster A_star. Will find node if node is adjazend to gate node, even if not in the same cluster


};


struct Hierarchy {
    std::vector<Level_CSR> levels;
    
    Hierarchy(Level_CSR level) : levels({std::move(level)}) {}
    Hierarchy(std::vector<Level_CSR> hierarchy) : levels(std::move(hierarchy)) {}



    //Methods

    //Build
    void build_next_level();

};
