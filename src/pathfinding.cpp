#include "phpa_types2.hpp"
#include "heap.hpp"

#include <math.h>


struct HeapNode {
    uint64_t node_id;
    float f_cost;
    float g_cost;
    bool operator<(const HeapNode& o) const { return f_cost < o.f_cost; }
};

Path Level_CSR::A_star(uint64_t start, uint64_t end) {
    uint64_t act_cluster = clusterID[start];
    uint64_t cluster_size = cluster_ptn[act_cluster+1] - cluster_ptn[act_cluster];
    uint64_t cluster_offset = cluster_ptn[act_cluster];
    
    std::vector<uint8_t> visited(cluster_size);
    
    NaryHeap<HeapNode, 8> queue(1024);


    queue.push({start, 0.0f, 0.0f});

    while(!queue.empty()) {
        HeapNode act = queue.pop();
        uint64_t node = act.node_id;
        if(node == end) { //found path

        }

        if(visited[node - cluster_offset]) continue;     //skip duplikat node

        
        
        visited[node - cluster_offset] = true;
        for(uint64_t edge = row_ptn[node]; edge < row_ptn[node+1]; edge++) {
            uint64_t neighbor = col_index[edge];
            if(neighbor != end && (neighbor < cluster_ptn[act_cluster] || neighbor >= cluster_ptn[act_cluster+1])) {
                continue;
            }

            float g = act.g_cost + weight[edge];
            
            float ds = 0;
            #pragma unroll
            for(size_t i = 0; i < Dim; i++) {
                float dd = (pos[node][i] - pos[end][i]);
                ds += dd*dd;
            }

            float h = sqrt(ds);
            queue.push({neighbor,g,h+g});
        }
        
    }
    
}