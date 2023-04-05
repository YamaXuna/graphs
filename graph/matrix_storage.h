//
// Created by USER on 02/04/2023.
//

#pragma once

#include <vector>
#include <array>
#include <optional>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include "common.h"

namespace xuna{

    /**
     *
     * @tparam Edge
     * @tparam Vertice
     */
    template<typename Vertice, typename Edge>
    class matrix_storage {
        using edges = std::optional<Edge>;

        std::unordered_map<Vertice, size_t> m_vertices;
        std::vector<std::vector<edges>> m_matrix;


        bool isVertice_present(const Vertice &v)const {
            return m_vertices.find(v) != end(m_vertices);
        }

    public:
        using vertice_t = Vertice;
        using edge_t = Edge;

        matrix_storage() = default;
        matrix_storage(matrix_storage &&m) noexcept =default;
        matrix_storage(matrix_storage &m) noexcept =default;
        matrix_storage<Vertice,  Edge> &operator=(const matrix_storage<Vertice,  Edge> &m)=default;
        matrix_storage<Vertice,  Edge> &operator=(matrix_storage<Vertice,  Edge> &&m) noexcept =default;
        ~matrix_storage() = default;

        /**
         *
         * @param v the vertice to add
         */
        template<typename V>
        void add(V &&v){
            m_vertices[std::forward<V>(v)] = std::size(m_vertices);

            for(auto &line : m_matrix){
                line.resize(std::size(m_vertices));
            }
            m_matrix.push_back(std::vector<edges>(std::size(m_vertices)));
        }

        /**
         *
         * @param source the source vertice
         * @param target the target vertice
         * @param edge the edge value
         */
        template<typename V, typename V2, typename E>
        void add(V &&source, V2 &&target, E &&edge){
            if(!isVertice_present(source))
                add(std::forward<V>(source));
            auto pos_a = m_vertices.find(source)->second;
            if(!isVertice_present(target))
                add(std::forward<V2>(target));
            auto pos_b = m_vertices.find(target)->second;

            m_matrix[pos_a][pos_b] = std::forward<E>(edge);
        }

        /**
         *
         * @param v the vertice to remove
         */
        void remove(const Vertice &v){
            auto it = m_vertices.find(v);
            if(it != end(m_vertices)){
                m_vertices.erase(it);
                m_matrix.erase(begin(m_matrix) + it->second);

                for(auto &vect : m_matrix){
                    vect.erase(begin(vect) + it->second);
                }
            }


        }
        /**
        *
        * @param source the source vertice
        * @param target the target vertice
        */
        void remove(const Vertice &source, const Vertice &target){
            if(!isVertice_present(source))
                throw VerticeDoesNotExistsError(source);
            auto pos_a = m_vertices.find(source)->second;
            if(!isVertice_present(target))
                throw VerticeDoesNotExistsError(target);
            auto pos_b = m_vertices.find(target)->second;

            m_matrix[pos_a][pos_b].reset();
        }
        /**
         *
         * @return the adjacency matrix
         */
        std::vector<std::vector<edges>> &matrix()noexcept{
            return m_matrix;
        }

        /**
         *
         * @return the vertices
         */
         //TODO replace it by an iterator
        std::vector<Vertice> vertices()noexcept{
            std::vector<Vertice> vect;
            for(auto &e : m_vertices){
                vect.push_back(e.first);
            }
            return vect;
        }
        std::vector<Vertice> vertices()const noexcept{
            std::vector<Vertice> vect;
            for(auto &e : m_vertices){
                vect.push_back(e.first);
            }
            return vect;
        }

        /**
         *
         * @param source the source vertice
         * @param target the target vertice
         * @return the optional containing the edge's value if it has been set
         * @throws if one vertice isn't present in the matrix
         */

        const edges &edge(const Vertice &source, const Vertice &target)const{
            if(!isVertice_present(source))
                throw VerticeDoesNotExistsError(source);
            auto pos_a = m_vertices.find(source)->second;
            if(!isVertice_present(target))
                throw VerticeDoesNotExistsError(target);
            auto pos_b = m_vertices.find(target)->second;

            return m_matrix[pos_a][pos_b];
        }

    };
}



