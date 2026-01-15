#pragma once

#include "phpa_types.hpp"
#include <limits>


template<std::size_t pos_dim = 2, typename T = int, typename indexType = uint32_t>
void bfs_clustering(CSRLevel<pos_dim, T, indexType>& graph, indexType max_node_count) {
    constexpr uint32_t NO_CLUSTER = std::numeric_limits<uint32_t>::max();
    std::fill(graph.clusterID.begin(), graph.clusterID.end(), NO_CLUSTER);

    uint32_t act_clusterID = 0;

    for(indexType act_seed = 0; act_seed < graph.node_count; ++act_seed) {
        if(graph.clusterID[act_seed] != NO_CLUSTER) continue;
        graph.clusterID[act_seed] = act_clusterID;

        //BFS
        std::vector<indexType> queue;
        queue.reserve(max_node_count);
        queue.push_back(act_seed);

        uint32_t cluster_size = 1;
        size_t queue_pointer = 0;

        while(queue_pointer < queue.size() && cluster_size < max_node_count) {

            indexType node = queue[queue_pointer++];
            indexType start = graph.row_ptr[node];
            indexType end = graph.row_ptr[node + 1];

            for(indexType e = start; e < end; ++e) {
                indexType nbr = graph.col_idx[e];

                if(graph.clusterID[nbr] == NO_CLUSTER) {
                    graph.clusterID[nbr] = act_clusterID;
                    queue.push_back(nbr);
                    ++cluster_size;
                    if(cluster_size >= max_node_count) {
                        break;
                    }
                }
            }
        }
        ++act_clusterID;
    }
}