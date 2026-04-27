#include "phpa_types2.hpp"
#include <array>

void Level_CSR::reorder() {
    std::vector<size_t> cluster_sizes(cluster_count);
    for(size_t i = 0; i < clusterID.size(); i++) {
        cluster_sizes[clusterID[i]]++;
    }

    cluster_ptn.resize(cluster_count+1);
    cluster_ptn[0] = 0;
    std::vector<uint64_t> heads(cluster_count);
    std::vector<uint64_t> tails(cluster_count);
    for(size_t i = 0; i < cluster_sizes.size();i++) {
        cluster_ptn[i+1] = cluster_ptn[i] + cluster_sizes[i];
        heads[i] = cluster_ptn[i];
        tails[i] = cluster_ptn[i+1] - 1;
    }
    cluster_sizes.clear();

    std::vector<uint64_t> permut(node_count);
    for(uint64_t i = 0; i < node_count; i++) {
        uint64_t clst = clusterID[i];
        if(is_gate[i]) {
            permut[i] = (heads[clst]++);
        } else {
            permut[i] = (tails[clst]--);
        }
    }
    heads.clear();
    tails.clear();

// Node Data
    std::vector<uint64_t> new_row_ptn(node_count+1);
    std::vector<uint64_t> new_clusterID(node_count);
    std::vector<uint8_t> new_is_gate(node_count); //wegen Cache?
    
    std::vector<std::array<uint32_t, Dim>> new_pos(node_count);
    
    std::vector<uint64_t> new_uplink(node_count);
    std::vector<uint64_t> new_downlink(node_count);


    // Edge Data
    std::vector<uint64_t> new_col_index(col_index.size());
    std::vector<float> new_weight(weight.size());

    std::vector<uint64_t> degree(node_count);

    for(uint64_t i = 0; i < node_count; i++) {
        degree[permut[i]] = row_ptn[i+1] - row_ptn[i];
    }


    new_row_ptn[0] = 0;
    for(size_t i = 0; i < degree.size(); i++) {
        new_row_ptn[i + 1] = new_row_ptn[i] + degree[i];
    }


    for(uint64_t i = 0; i < node_count; i++) {
        

        //Nodes
        new_clusterID[permut[i]] = clusterID[i];
        new_is_gate[permut[i]] = is_gate[i];
        new_pos[permut[i]] = pos[i];

        if(level != 0) {
            new_downlink[permut[i]] = downlink[i];
        }

        //Edges
        for(uint64_t e = row_ptn[i], j = 0; e < row_ptn[i + 1]; e++, j++) {
            new_col_index[new_row_ptn[permut[i]] + j] = permut[col_index[e]];
            new_weight[new_row_ptn[permut[i]] + j] = weight[e];
        }
    }

    row_ptn   = std::move(new_row_ptn);
    clusterID = std::move(new_clusterID);
    is_gate   = std::move(new_is_gate);
    pos       = std::move(new_pos);
    col_index = std::move(new_col_index);
    weight    = std::move(new_weight);
    if(level != 0)
        downlink = std::move(new_downlink);


}   