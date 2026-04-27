#include "phpa_types2.hpp"

#include <cstring>

void Level_CSR::cluster() {
    
    memset(clusterID.data(), 0, clusterID.size() * sizeof(uint64_t));
    memset(is_gate.data(), 1, is_gate.size() * sizeof(uint8_t));

    size_t seed = 0;
    size_t cluster_coutner = 0;




    if(Level_CSR::level == 0) {
        
        while(seed < clusterID.size()) {

            while (seed < clusterID.size() && clusterID[seed] != 0) {
                seed++;
            }
            if (seed >= clusterID.size()) break;



            cluster_coutner++;
            std::array<uint64_t, max_cluster_size> cluster;
            size_t act_cluster_siez = 0;

            cluster[0] = seed;
            clusterID[seed] = cluster_coutner;
            
            size_t head = 0;
            size_t tail = 1;

            while(tail < max_cluster_size && head < tail) {
                uint64_t node = cluster[head++];
                bool external_edge = false;

                for(uint64_t i = row_ptn[node]; i < row_ptn[node + 1]; i++) {
                    uint64_t neighbor = col_index[i];
                    if (clusterID[neighbor] == 0 && tail < max_cluster_size) {
                        clusterID[neighbor] = cluster_coutner;
                        cluster[tail++] = neighbor;
                    } else if(clusterID[neighbor] != cluster_coutner) {
                        external_edge = true;
                    }
                }
                is_gate[node] = external_edge;
            }

            seed++;
            
        }
            
    } else {
            //WIP:
            // Hier muss mit dem Downlink auf die Verschiedenen Cluster zugegriffen werden, und diese zu dem Cluster hinzu gefügt werden.
            // Dafür kann man Feststellen ob ein neuer Cluster getroffen wurde und dann alle Knoten in dem Cluster die mit
            // is_gate[..] == 1 gekennzeinet sind über den Uplink wieder die richtige Cluter ID zuweisen
    }



    Level_CSR::cluster_count = cluster_coutner;

}

