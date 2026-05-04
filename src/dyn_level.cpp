#include "phpa_types2.hpp"


void Level_CSR::add_node(
    std::initializer_list<uint64_t> neighbors,
    std::initializer_list<double> weights,
    std::initializer_list<std::array<uint32_t, Dim>> positions
) {
    // neue Kanten anhängen
    for(uint64_t n : neighbors) col_index.push_back(n);
    for(double w : weights)     weight.push_back(w);
    
    // row_ptn für neuen Knoten
    row_ptn.push_back(row_ptn.back() + neighbors.size());
    
    // Position
    for(auto& p : positions) pos.push_back(p);
}

