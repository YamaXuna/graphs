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
            using std::begin, std::end;
            return m_vertices.find(v) != end(m_vertices);
        }

    public:

        class vertex_iterator {
            using iterator = typename std::unordered_map<Vertice, size_t>::const_iterator;
            iterator m_it;

        public:
            using difference_type = std::ptrdiff_t;
            using value_type = Vertice;
            using pointer = const Vertice*;
            using reference = const Vertice&;
            using iterator_category = std::forward_iterator_tag;

            vertex_iterator() = default;
            explicit vertex_iterator(iterator it) : m_it(it) {}

            reference operator*() const {
                return m_it->first;
            }

            pointer operator->() const {
                return &m_it->first;
            }

            vertex_iterator& operator++() {
                ++m_it;
                return *this;
            }

            vertex_iterator operator++(int) {
                vertex_iterator temp = *this;
                ++(*this);
                return temp;
            }

            bool operator==(const vertex_iterator& other) const {
                return m_it == other.m_it;
            }

            bool operator!=(const vertex_iterator& other) const {
                return !(*this == other);
            }
        };

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
            using std::begin, std::end;
            auto it = m_vertices.find(v);
            if(it != end(m_vertices)){
                m_vertices.erase(it);
                std::cout << it->second << '\n';
                std::cout << std::size(m_matrix) << '\n';
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
        const std::vector<std::vector<edges>> &matrix()const noexcept{
            return m_matrix;
        }

        /**
         *
         * @return the vertices
         */

        vertex_iterator begin()const noexcept{
            return vertex_iterator(std::begin(m_vertices));
        }
        vertex_iterator end()const noexcept{
            return vertex_iterator(std::end(m_vertices));
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



