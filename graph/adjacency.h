//
// Created by USER on 01/02/2024.
//

#pragma once

#include "common.h"
#include <unordered_map>
#include <unordered_set>

namespace xuna {

    template<typename Vertice, typename Edge>
    class adjacency_list {
        using map_t = std::unordered_map<Vertice, std::unordered_set<Edge>>;
        map_t list;

    public:
        using edge_t = Edge;
        using vertice_t = Vertice;


        adjacency_list() = default;
        adjacency_list(const adjacency_list &other) = default;
        adjacency_list(adjacency_list &&other) noexcept = default;


        template<typename V>
        void add(V &&vertice){
            if(list.contains(vertice))
                return;

            list[std::forward<V>(vertice)] = std::unordered_set<Edge>{};
        }

    };

} // xuna

