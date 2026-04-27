#pragma once

#include <vector>
#include <array>
#include <cstdint>

static constexpr size_t Dim = 2;
static constexpr size_t max_cluster_size = 256;


struct Level_CSR {

    // Meta Data
    uint64_t cluster_count;
    uint64_t node_count;
    uint64_t edge_count;
    uint64_t level;
    Level_CSR* prev_level;
    Level_CSR* next_level;


    // Cluster Data
    std::vector<uint64_t> cluster_ptn;
    

    // Node Data
    std::vector<uint64_t> row_ptn;
    std::vector<uint64_t> clusterID;
    std::vector<uint8_t> is_gate; //wegen Cache?
    
    std::vector<std::array<uint32_t, Dim>> pos;
    
    std::vector<uint64_t> uplink;
    std::vector<uint64_t> downlink;


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

    void build_level();

};
