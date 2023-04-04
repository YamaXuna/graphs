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
    template<typename Vertice, typename Edge>
    class graph {
        using storage_t = matrix_storage<Vertice,  Edge>;

        storage_t storage;

    public:
        graph()=default;
        graph(graph &g)noexcept = default;
        graph(graph &&g)noexcept = default;
        graph<Vertice,  Edge> &operator=(const graph<Vertice,  Edge> g)=default;
        graph<Vertice,  Edge> &operator=(graph<Vertice,  Edge> &&g) noexcept =default;
        ~graph()=default;

        /**
         *
         * @param v the vertice to add
         */
        template<typename V>
        void add(V &&v){
            storage.add(std::forward<V>(v));
        }
        /**
         *
         * @param source the source vertice
         * @param target the target vertice
         * @param edge the edge value
         */
        template<typename V, typename V2, typename E>
        void add(V &&source, V2 &&target, E &&edge){
            storage.add(std::forward<V>(source), std::forward<V2>(target), std::forward<E>(edge));
        }
        /**
         *
         * @param v the vertice to remove
         */
        template<typename V>
        void remove(V &&v){
            storage.remove(std::forward<V>(v));
        }
        /**
        *
        * @param source the source vertice
        * @param target the target vertice
        */
        template<typename V, typename V2>
        void remove(V &&source, V2 &&target){
            storage.remove(std::forward<V>(source), std::forward<V2>(target));
        }
        /**
         *
         * @param source the source vertice
         * @param target the target vertice
         * @return the optional containing the edge's value if it has been set
         * @throws if one vertice isn't present in the graph
         */
        template<typename V, typename V2>
        storage_t::edge_t edge(V &&source, V2 &&target)const{
            storage.edge(std::forward<V>(source), std::forward<V2>(target));
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

