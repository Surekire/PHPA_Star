#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "clustering.hpp"

TEST_CASE("one node") {
    CSRLevel graph;

    graph.col_idx = {};
    graph.row_ptr = {0,0};

    graph.clusterID[0] = 1;

    bfs_clustering(graph, 1U);

    CHECK(graph.clusterID[0] == 0);

    CHECK(graph.clusterID.size() == 1);
}

TEST_CASE("one cluster") {
    CSRLevel graph;

    graph.col_idx = {1,2,0,0};
    graph.row_ptr = {0,2,3,4};

    bfs_clustering(graph, 3U);

    CHECK(graph.clusterID[0] == 0);
    CHECK(graph.clusterID[1] == 0);
    CHECK(graph.clusterID[2] == 0);

    CHECK(graph.clusterID.size() == 1);   
}

TEST_CASE("3-one cluster") {
    CSRLevel graph;

    graph.col_idx = {1,2,0,0};
    graph.row_ptr = {0,2,3,4};

    bfs_clustering(graph, 1U);

    CHECK(graph.clusterID[0] == 0);
    CHECK(graph.clusterID[1] == 1);
    CHECK(graph.clusterID[2] == 2);

    CHECK(graph.clusterID.size() == 3);
}

TEST_CASE("2 cluster") {
    CSRLevel graph;

    graph.col_idx = {1, 2, 0, 3, 0, 3, 4, 1, 2, 4, 2};
    graph.row_ptr = {0, 2, 4, 7, 10, 11};

    bfs_clustering(graph, 3U);

    CHECK(graph.clusterID[0] == 0);
    CHECK(graph.clusterID[1] == 0);
    CHECK(graph.clusterID[2] == 0);
    CHECK(graph.clusterID[3] == 1);
    CHECK(graph.clusterID[4] == 1);

    CHECK(graph.clusterID.size());
}