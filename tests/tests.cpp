//
// Created by USER on 22/04/2023.
//

#include "graph_testor.h"
#include "graph/graph.h"
#include "graph/algorithms.h"

#include <iostream>

template<template<typename, typename> typename Graph>
inline void testor(){
    xuna::graph_testor<Graph>{}();
}

int main(){
    /*
    xuna::graph_wrapper<xuna::matrix_storage<int, int>> g;
    std::cout << "ddd\n";
    testor<xuna::matrix_storage>();
     */
    xuna::adjacency_list<int, std::string> g;
    g.add(12);

}
