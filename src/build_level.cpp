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
    Level_CSR& prev = levels.back();
    levels.emplace_back();
    Level_CSR& next = levels.back();

    for(uint64_t i = 0; i < prev.cluster_ptn.size(); i++) {
        uint64_t act_cluster = prev.cluster_ptn[i];

    }


}