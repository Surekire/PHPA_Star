#include "phpa_types2.hpp"


void Hierarchy::build_next_level() {
    /*Was muss ich hier machen???
        1. Clusterweise Durch gehen:
        1.1. Gate finden, mit allen Vorherigen gates das Clusters verbinden, hin und rück kannte einfügen
        1.2. alles in einer Temporeren Struktur speichern.
        2. Temporäre daten in CSR schreiben.
        3. Clustern
        3.1. hier muss das funktionieren dass die cluster nicht zerreißen
        4. Reodern
    */

    
    //Neuen CSR Anlegen
    levels.reserve(levels.size() + 1);
    Level_CSR& prev = levels.back();
    levels.emplace_back();
    Level_CSR& next = levels.back();

    next.cluster_ptn.push_back(0);
    next.row_ptn.push_back(0);

    for(uint64_t i = 0; i < prev.cluster_ptn.size()-1; i++) {
        uint64_t cluster_offset = prev.cluster_ptn[i];
        
        std::vector<std::vector<uint64_t>> connection;
        std::vector<std::vector<uint64_t>> weights;

        for(uint64_t n = prev.cluster_ptn[i]; n < prev.cluster_ptn[i+1]; n++) {
            if(!prev.is_gate[n]) continue;; //go on if all gates of the cluster are processed
            //here could be a brake since all gates are in the front after reorder
            

            prev.uplink.insert({n,next.node_count});
            next.downlink.push_back(n);
            next.node_count++;
            connection.push_back({});
            weights.push_back({});

            for(uint64_t b = prev.cluster_ptn[i]; b < n; b++) {
                Path p = prev.A_star(b,n);
                if(p.distances.size() == 0) continue; //skip if no path found in cluster;
                next.edge_count += 2; //since we push both direction seperatly

                connection[b - cluster_offset].push_back(n);
                connection[n - cluster_offset].push_back(b);
                weights[b - cluster_offset].push_back(p.total_distance);
                weights[n - cluster_offset].push_back(p.total_distance);

                
                
            }

            for(uint64_t e = prev.row_ptn[n]; e < prev.row_ptn[n+1]; e++) {
                uint64_t neighbor = prev.col_index[e];
                if(prev.clusterID[neighbor] == prev.clusterID[n]) continue; //no internal edges will get copyed over

                connection[n - cluster_offset].push_back(neighbor);
                weights[n - cluster_offset].push_back(prev.weight[e]);
            }
        }   
        
        for(uint64_t n = 0; n < connection.size(); n++) {
            next.row_ptn.push_back(connection[n].size() + next.row_ptn.back());
            
            for(uint64_t d = 0; d < connection[n].size(); d++) {
                
                next.col_index.push_back(connection[n][d]);
                next.weight.push_back(weights[n][d]);
            }
        }

    }


    //rewrites old IDs as new ones
    for(uint64_t i = 0; i < next.col_index.size(); i++) {
        next.col_index[i] = prev.uplink[next.col_index[i]];
    }



}