//
// Created by USER on 01/04/2023.
//

#pragma once

//#include "graph.h"
#include "matrix_storage.h"


namespace xuna {


    /**
     * wrapper for any graph class, only expose primary operations
     * @tparam Edge
     * @tparam Vertice
     */
    template<typename Vertice, typename Edge>
    class graph_wrapper {
        using storage_t = matrix_storage<Vertice,  Edge>;

        storage_t storage;

    public:
        graph_wrapper()=default;
        graph_wrapper(graph_wrapper &g)noexcept = default;
        graph_wrapper(graph_wrapper &&g)noexcept = default;
        graph_wrapper<Vertice,  Edge> &operator=(const graph_wrapper<Vertice,  Edge> g)=default;
        graph_wrapper<Vertice,  Edge> &operator=(graph_wrapper<Vertice,  Edge> &&g) noexcept =default;
        ~graph_wrapper()=default;

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
         * @throws if one vertice isn't present in the graph_wrapper
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
