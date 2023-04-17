//
// Created by USER on 05/04/2023.
//

#pragma once

#include <type_traits>
//#include "graph_wrapper.h"

/**
*
* @tparam Graph the graph, it must have type traits for both vertices and edges
*/
template<typename Graph>
concept graph = requires(Graph g, typename Graph::vertice_t v, typename Graph::edge_t e){
    g.add(typename Graph::vertice_t{});
    g.add(typename Graph::vertice_t{}, typename Graph::vertice_t{}, typename Graph::edge_t{});
    {g.edge(typename Graph::vertice_t{}, typename Graph::vertice_t{})} -> std::same_as<std::optional<typename Graph::edge_t> const &>;
    g.remove(typename Graph::vertice_t{});
    g.remove(typename Graph::vertice_t{}, typename Graph::vertice_t{});
    {begin(g)} -> std::same_as<typename Graph::vertex_iterator>;
    {end(g)} -> std::same_as<typename Graph::vertex_iterator>;

};

/**
 *
 * @tparam Vertice
 */
template<typename Vertice>
struct VerticeDoesNotExistsError : public std::runtime_error{
    explicit VerticeDoesNotExistsError(const Vertice &v): std::runtime_error{"Vertice isn't in the graph"}{}
};