#include "phpa_types2.hpp"

#include <cstring>


void Level_CSR::cluster(Level_CSR& prev) {
    
    memset(clusterID.data(), 0, clusterID.size() * sizeof(uint64_t));
    memset(is_gate.data(), 1, is_gate.size() * sizeof(uint8_t));

    size_t seed = 0;
    size_t cluster_counter = 0;




    if(Level_CSR::level == 0) {
        
        while(seed < clusterID.size()) {

            while (seed < clusterID.size() && clusterID[seed] != 0) {
                seed++;
            }
            if (seed >= clusterID.size()) break;



            cluster_counter++;
            std::array<uint64_t, max_cluster_size> cluster;

            cluster[0] = seed;
            clusterID[seed] = cluster_counter;
            
            size_t head = 0;
            size_t tail = 1;

            while(tail < max_cluster_size && head < tail) {
                uint64_t node = cluster[head++];
                bool external_edge = false;

                for(uint64_t i = row_ptn[node]; i < row_ptn[node + 1]; i++) {
                    uint64_t neighbor = col_index[i];
                    if (clusterID[neighbor] == 0 && tail < max_cluster_size) {
                        clusterID[neighbor] = cluster_counter;
                        cluster[tail++] = neighbor;
                    } else if(clusterID[neighbor] != cluster_counter) {
                        external_edge = true;
                    }
                }
                is_gate[node] = external_edge;
                gates += external_edge;
            }
        }
            
    } else {
        //WIP:
        // Hier muss mit dem Downlink auf die Verschiedenen Cluster zugegriffen werden, und diese zu dem Cluster hinzu gefügt werden.
        // Dafür kann man Feststellen ob ein neuer Cluster getroffen wurde und dann alle Knoten in dem Cluster die mit
        // is_gate[..] == 1 gekennzeinet sind über den Uplink wieder die richtige Cluter ID zuweisen

        while(seed < clusterID.size()) {

            while (seed < clusterID.size() && clusterID[seed] != 0) {
                seed++;
            }
            if (seed >= clusterID.size()) break;



            cluster_counter++;
            std::array<uint64_t, max_cluster_size> cluster;

            size_t head = 0;
            size_t tail = 0;
            
            uint64_t prev_cluster_id = prev.clusterID[downlink[seed]];
            uint64_t cluster_start = prev.cluster_ptn[prev_cluster_id - 1];
            uint64_t cluster_end   = prev.cluster_ptn[prev_cluster_id];
            for(uint64_t i = cluster_start; i < cluster_end; i++) {
                if(!prev.is_gate[i]) { break; }
                clusterID[prev.uplink[i]] = cluster_counter;
                cluster[tail++] = prev.uplink[i];
            }


            while(tail < max_cluster_size && head < tail) {
                uint64_t node = cluster[head++];
                bool external_edge = false;

                for(uint64_t i = row_ptn[node]; i < row_ptn[node + 1]; i++) {
                    uint64_t neighbor = col_index[i];
                    if (clusterID[neighbor] == 0) {
                        // neighbor gehört noch keinem Cluster auf dieser Ebene
                        uint64_t neighbor_prev_id = prev.clusterID[downlink[neighbor]];
                        uint64_t n_start = prev.cluster_ptn[neighbor_prev_id - 1];
                        uint64_t n_end   = prev.cluster_ptn[neighbor_prev_id];

                        uint64_t cc = 0;
                        for(uint64_t j = n_start; j < n_end; j++) {
                            if(!prev.is_gate[j]) break; 
                            cc++;
                        }

                        if(tail + cc <= max_cluster_size) {
                            for(uint64_t j = n_start; j < n_start + cc; j++) {
                                clusterID[prev.uplink[j]] = cluster_counter;
                                cluster[tail++] = prev.uplink[j];
                            }
                        } else {
                            external_edge = true;
                        }
                    } else if (clusterID[neighbor] != cluster_counter) {
                        external_edge = true;
                    }
                }
                is_gate[node] = external_edge;
                gates += external_edge;
            }

            
        }

    }



    Level_CSR::cluster_count = cluster_counter;

}

