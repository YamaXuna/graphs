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
    template<typename Graph> requires graph<Graph>
    class graph_wrapper {
        using storage_t = Graph;

        storage_t m_storage;

    public:
        using Vertice = Graph::vertice_t;
        using Edge = Graph::edge_t;
        using vertice_t = Graph::vertice_t;
        using edge_t = Graph::edge_t;
        using vertex_iterator = Graph::vertex_iterator;

        graph_wrapper()=default;
        graph_wrapper(graph_wrapper &g)noexcept = default;
        graph_wrapper(graph_wrapper &&g)noexcept = default;
        graph_wrapper<Graph> &operator=(const graph_wrapper<Graph> &g)=default;
        graph_wrapper<Graph> &operator=(graph_wrapper<Graph> &&g) noexcept =default;
        ~graph_wrapper()=default;

        /**
         *
         * @param v the vertice to add
         */
        template<typename V>
        void add(V &&v){
            m_storage.add(std::forward<V>(v));
        }
        /**
         *
         * @param source the source vertice
         * @param target the target vertice
         * @param edge the edge value
         */
        template<typename V, typename V2, typename E>
        void add(V &&source, V2 &&target, E &&edge){
            m_storage.add(std::forward<V>(source), std::forward<V2>(target), std::forward<E>(edge));
        }
        /**
         *
         * @param v the vertice to remove
         */
        template<typename V>
        void remove(V &&v){
            m_storage.remove(std::forward<V>(v));
        }
        /**
        *
        * @param source the source vertice
        * @param target the target vertice
        */
        template<typename V, typename V2>
        void remove(V &&source, V2 &&target){
            m_storage.remove(std::forward<V>(source), std::forward<V2>(target));
        }
        /**
         *
         * @param source the source vertice
         * @param target the target vertice
         * @return the optional containing the edge's value if it has been set
         * @throws if one vertice isn't present in the graph_wrapper
         */
        template<typename V, typename V2>
        const std::optional<Edge> &edge(V &&source, V2 &&target)const{
            return m_storage.edge(std::forward<V>(source), std::forward<V2>(target));
        }

        template<typename V>
        std::vector<std::reference_wrapper<const Vertice>> neighbours(V &&v)const{
            return m_storage.neighbours(std::forward<V>(v));
        }

        /**
         *
         * @return the vertices
         */
        decltype(m_storage.begin()) begin()const noexcept{
            return m_storage.begin();
        }

        decltype(m_storage.end()) end()const noexcept{
            return m_storage.end();
        }
    };

} // xuna

