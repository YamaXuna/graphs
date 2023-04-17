//
// Created by USER on 17/04/2023.
//

#pragma once
#include "common.h"
#include <queue>

namespace xuna{

    template<typename Graph>
    void breadth_first_search(Graph &g) requires graph<Graph>{
        using vertice_t = typename Graph::vertice_t;
        std::queue<vertice_t> visited;


    }
}
