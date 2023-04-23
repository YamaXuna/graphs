//
// Created by USER on 17/04/2023.
//

#pragma once
#include "common.h"
#include <queue>
#include <stack>
#include <unordered_set>

namespace xuna{
    template<typename Graph, typename F>
    void breadth_first_search(Graph &g, F f, const typename Graph::vertex_iterator &start) requires graph<Graph> {
        using vertice_t = typename Graph::vertice_t;

        auto vertice_hash = [](const vertice_t& v) {
            return std::hash<vertice_t>()(v);
        };
        auto vertice_equal = get_comparator<vertice_t>();
        std::unordered_set<std::reference_wrapper<const vertice_t>, decltype(vertice_hash), decltype(vertice_equal)> visited(0, vertice_hash, vertice_equal);

        std::queue<std::reference_wrapper<const vertice_t>> queue;

        auto it = start;
        queue.push(std::cref(*it));
        visited.emplace(std::cref(*it));

        while (!queue.empty()) {

            const vertice_t& current = queue.front().get();
            queue.pop();
            f(current);

            for (const auto& neighbour : g.neighbours(current)) {
                if (visited.find(neighbour) == end(visited)) {
                    queue.push(neighbour);
                    visited.emplace(neighbour);
                }
            }
        }
    }

    template<typename Graph, typename F>
    void breadth_first_search(Graph &g, F f) requires graph<Graph> {
        breadth_first_search(g, f, begin(g));
    }


    template<typename Graph, typename F>
    void depth_first_search(Graph &g, F f, const typename Graph::vertex_iterator &start) requires graph<Graph> {
        using vertice_t = typename Graph::vertice_t;

        auto vertice_hash = [](const std::reference_wrapper<const vertice_t>& v) {
            return std::hash<vertice_t>()(v.get());
        };
        auto vertice_equal = [](const std::reference_wrapper<const vertice_t>& v1, const std::reference_wrapper<const vertice_t>& v2) {
            return std::equal_to<vertice_t>()(v1.get(), v2.get());
        };
        std::unordered_set<std::reference_wrapper<const vertice_t>, decltype(vertice_hash), decltype(vertice_equal)> visited(0, vertice_hash, vertice_equal);

        std::stack<std::reference_wrapper<const vertice_t>> stack;
        auto it = start;
        stack.push(std::cref(*it));
        visited.emplace(std::cref(*it));

        while (!stack.empty()) {
            const vertice_t& current = stack.top().get();
            stack.pop();
            f(current);

            for (const auto& neighbour : g.neighbours(current)) {
                if (visited.find(neighbour) == end(visited)) {
                    stack.push(neighbour);
                    visited.emplace(neighbour);
                }
            }
        }
    }

    template<typename Graph, typename F>
    void depth_first_search(Graph &g, F f) requires graph<Graph> {
        depth_first_search(g, f, begin(g));
    }

}
