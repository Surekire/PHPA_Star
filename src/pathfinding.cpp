#include "phpa_types2.hpp"
#include "heap.hpp"

#include <math.h>
#include <algorithm>

struct HeapNode {
    uint64_t node_id;
    uint64_t prev;
    float f_cost;
    float g_cost;
    bool operator<(const HeapNode& o) const { return f_cost < o.f_cost; }
};

Path Level_CSR::A_star(uint64_t start, uint64_t end) {
    uint64_t act_cluster = clusterID[start];
    uint64_t cluster_size = cluster_ptn[act_cluster+1] - cluster_ptn[act_cluster];
    uint64_t cluster_offset = cluster_ptn[act_cluster];
    
    std::vector<uint8_t> visited(cluster_size);
    std::vector<HeapNode> prev(cluster_size);
    
    NaryHeap<HeapNode, 8> queue(1024);


    queue.push({start, start, 0.0f, 0.0f});

    while(!queue.empty()) {
        HeapNode act = queue.pop();
        uint64_t node = act.node_id;
        if(node == end) { //found path
            Path out;
            out.total_distance = act.g_cost;

            while(act.node_id != start) {
                out.add(act.node_id,act.g_cost);
                act = prev[act.prev - cluster_offset];
            }
            
            out.add(act.node_id, 0.0f);
            std::reverse(out.nodes.begin(), out.nodes.end());
            std::reverse(out.distances.begin(), out.distances.end());
            return out;
        }

        if(visited[node - cluster_offset]) continue;     //skip duplikat node

        
        
        visited[node - cluster_offset] = true;
        prev[node - cluster_offset] = act;

        for(uint64_t edge = row_ptn[node]; edge < row_ptn[node+1]; edge++) {
            uint64_t neighbor = col_index[edge];

            //Skip out of cluster nodes
            if(neighbor != end && (neighbor < cluster_ptn[act_cluster] || neighbor >= cluster_ptn[act_cluster+1])) continue; 

            float g = act.g_cost + weight[edge];
            
            float ds = 0;
            #pragma unroll
            for(size_t i = 0; i < Dim; i++) {
                float dd = (pos[node][i] - pos[end][i]);
                ds += dd*dd;
            }

            float h = sqrt(ds);
            queue.push({neighbor, node,h+g,g});
        }
        
    }

    //nothing found
    return {};
    
}