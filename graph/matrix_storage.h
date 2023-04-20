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


        template<typename T>
        using comparator_t = std::function<bool(const T&, const T&)>;

        template<typename T>
        comparator_t<T> get_comparator() {
            if constexpr (is_ptr<T>::value) {
                return [](const auto& lhs, const auto& rhs) {
                    return *lhs == *rhs;
                };
            } else {
                return [](const auto& lhs, const auto& rhs) -> bool {
                    return lhs == rhs;
                };
            }
        }




        template<typename T>
        std::enable_if_t<!is_ptr<T>::value, std::optional<size_t>>
        find_Vertice(const T& v) const {
            if(auto it = m_vertices.find(v); it != m_vertices.end()){
                return std::optional(it->second);
            }
            return std::nullopt;
        }



        template<typename T>
        std::enable_if_t<is_ptr<T>::value, std::optional<size_t>>
        find_Vertice(const T& v) const {
            for (const auto& vertex : m_vertices) {
                if (*v == *vertex.first) {
                    return std::optional(vertex.second);
                }
            }
            return std::nullopt;
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
            auto v = find_Vertice(source);
            size_t pos_a;
            if(!v)
            {
                add(std::forward<V>(source));
                pos_a = std::size(m_vertices);
            }
            else
            {
                pos_a = *v;
            }
            auto v2 = find_Vertice(target);
            size_t pos_b;
            if(!v2)
            {
                add(std::forward<V2>(target));
                pos_b = std::size(m_vertices);
            }
            else
            {
                pos_b = *v2;
            }
            //std::cout << pos_a << ' ' << pos_b << '\n';
            m_matrix[pos_a][pos_b] = std::forward<E>(edge);
        }

        /**
         *
         * @param v the vertice to remove
         */
        void remove(const Vertice &v){
            using std::begin, std::end;
            auto comp = get_comparator<Vertice>();

            /*auto it = std::find_if(begin(m_vertices), end(m_vertices), [comp, &v](const auto &pair){
                return comp(v, pair.first);
            });*/
            auto it = m_vertices.find(v);
            if(it != end(m_vertices)){
                size_t pos = it->second;
                m_vertices.erase(it);
                m_matrix.erase(begin(m_matrix) + pos);

                for(auto &vect : m_matrix){
                    vect.erase(begin(vect) + pos);
                }
            }
        }
        /**
        *
        * @param source the source vertice
        * @param target the target vertice
        */
        void remove(const Vertice &source, const Vertice &target){
            auto v = find_Vertice(source);
            if(!v)
                throw VerticeDoesNotExistsError(source);
            size_t pos_a = *v;
            auto v2 = find_Vertice(target);
            if(!v2)
                throw VerticeDoesNotExistsError(target);
            size_t pos_b = *v2;

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
            auto v = find_Vertice(source);
            if(!v)
                throw VerticeDoesNotExistsError(source);
            size_t pos_a = *v;
            auto v2 = find_Vertice(target);
            if(!v2)
                throw VerticeDoesNotExistsError(target);
            size_t pos_b = *v2;

            return m_matrix[pos_a][pos_b];
        }


    };
}



