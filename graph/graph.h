//
// Created by USER on 01/04/2023.
//

#pragma once

#include "matrix_storage.h"
#include <type_traits>

namespace xuna {

    /**
     *
     * @tparam Graph the graph, it must have type traits for both vertices and edges
     */
    template<typename Graph>
    concept graph_storage = requires(Graph g, typename Graph::vertice_t v, typename Graph::edge_t e){
        g.add(typename Graph::vertice_t{});
        g.add(typename Graph::vertice_t{}, typename Graph::vertice_t{}, typename Graph::edge_t{});
        {g.edge(typename Graph::vertice_t{}, typename Graph::vertice_t{})} -> std::same_as<std::optional<typename Graph::edge_t>>;
        g.remove(typename Graph::vertice_t{});
        g.remove(typename Graph::vertice_t{}, typename Graph::vertice_t{});

    };

    /**
     * wrapper for any graph class, only expose primary operations
     * @tparam Edge
     * @tparam Vertice
     */
    template<typename Edge, typename Vertice>
    class graph {
        using storage_t = matrix_storage<Edge, Vertice>;

        storage_t storage;

    public:
        graph()=default;
        graph(graph &g)noexcept = default;
        graph(graph &&g)noexcept = default;
        graph<Edge, Vertice> &operator=(const graph<Edge, Vertice> g)=default;
        graph<Edge, Vertice> &operator=(graph<Edge, Vertice> &&g) noexcept =default;
        ~graph()=default;

        /**
         *
         * @param v the vertice to add
         */
        void add(Vertice &&v){
            storage.add(std::forward<Vertice>(v));
        }
        /**
         *
         * @param source the source vertice
         * @param target the target vertice
         * @param edge the edge value
         */
        void add(Vertice &&source, Vertice &&target, Edge &&edge){
            storage.add(std::forward<Vertice>(source), std::forward<Vertice>(target), std::forward<Edge>(edge));
        }
        /**
         *
         * @param v the vertice to remove
         */
        void remove(Vertice &&v){
            storage.remove(std::forward<Vertice>(v));
        }
        /**
        *
        * @param source the source vertice
        * @param target the target vertice
        */
        void remove(Vertice &&source, Vertice &&target){
            storage.remove(std::forward<Vertice>(source), std::forward<Vertice>(target));
        }
        /**
         *
         * @param source the source vertice
         * @param target the target vertice
         * @return the optional containing the edge's value if it has been set
         * @throws if one vertice isn't present in the graph
         */
        storage_t::edge_t edge(Vertice &&source, Vertice &&target)const{
            storage.edge(std::forward<Vertice>(source), std::forward<Vertice>(target));
        }
        /**
         *
         * @return the vertices
         */
        std::vector<std::reference_wrapper<Vertice>> vertices()noexcept{
            return storage.vertices();
        }
    };

} // xuna

