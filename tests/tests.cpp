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
    testor<xuna::matrix_storage>();
}
