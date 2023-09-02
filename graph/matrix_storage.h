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


        decltype(m_vertices.cbegin()) find_vertice(const Vertice &v)const{
            auto comp = get_comparator<Vertice>();
            return std::find_if(cbegin(m_vertices), cend(m_vertices), [comp, &v](const auto &pair){
                return comp(v, pair.first);
            });
        }

        decltype(m_vertices.begin()) find_vertice(const Vertice &v){
            using std::begin, std::end;
            auto comp = get_comparator<Vertice>();
            return std::find_if(begin(m_vertices), end(m_vertices), [comp, &v](const auto &pair){
                return comp(v, pair.first);
            });
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
            using std::begin, std::end;
            auto it = find_vertice(source);
            size_t pos_a;
            if(it == end(m_vertices)){
                add(std::forward<V>(source));
                pos_a = std::size(m_vertices) - 1;
            } else{
                pos_a = it->second;
            }

            auto it2 = find_vertice(target);
            size_t pos_b;
            if(it2 == end(m_vertices)){
                add(std::forward<V2>(target));
                pos_b = std::size(m_vertices) - 1;
            }else{
                pos_b = it2->second;
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
            auto it = find_vertice(v);
            if(it != end(m_vertices)){
                const size_t pos = it->second;
                for(auto &pair : m_vertices){
                    if (pair.second != pos){
                        auto &vect = m_matrix[pair.second];
                        vect.erase(begin(vect) + pos);
                    }
                    if(pair.second > pos)
                        --pair.second;
                }
                m_vertices.erase(it);
                m_matrix.erase(begin(m_matrix) + pos);
            }
        }
        /**
        *
        * @param source the source vertice
        * @param target the target vertice
        */
        void remove(const Vertice &source, const Vertice &target){
            using std::end;
            auto it = find_vertice(source);
            if(it == end(m_vertices))
                throw VerticeDoesNotExistsError(source);
            size_t pos_a = it->second;
            auto it2 = find_vertice(target);
            if(it2 == end(m_vertices))
                throw VerticeDoesNotExistsError(target);
            size_t pos_b = it2->second;
            //std::cout << pos_a << ' ' << pos_b << '\n';
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
        vertex_iterator end() noexcept{
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
            auto it = find_vertice(source);
            if(it == cend(m_vertices))
                throw VerticeDoesNotExistsError(source);
            size_t pos_a = it->second;
            auto it2 = find_vertice(target);
            if(it2 == cend(m_vertices))
                throw VerticeDoesNotExistsError(target);
            size_t pos_b = it2->second;

            return m_matrix[pos_a][pos_b];
        }
        /**
         * give the neighbours of a vertice
         * @param v the vertice
         * @return the vector of references to the neighbours
         */
         //TODO return also the weight of the eges to the neighbours
        std::vector<std::pair<
                std::reference_wrapper<const Vertice>, std::reference_wrapper<const Edge>
                >> neighbours(const Vertice &v)const {
            auto it = find_vertice(v);
             std::vector<std::pair<
                     std::reference_wrapper<const Vertice>, std::reference_wrapper<const Edge>
             >> vect;
            if (it == cend(m_vertices))
                throw VerticeDoesNotExistsError(v);
            for (const auto &pair: m_vertices) {
                if (const auto &weight = m_matrix[it->second][pair.second]; weight.has_value()) {
                    auto &vertex = pair.first;
                    vect.emplace_back(std::cref(vertex), std::cref(*m_matrix[it->second][pair.second]));
                }
            }
            return vect;
        }

    };

    template<typename Vertice, typename Edge>
    matrix_storage<Vertice, Edge>::vertex_iterator begin(const matrix_storage<Vertice, Edge> &graph) noexcept{
        return graph.begin();
    }
    template<typename Vertice, typename Edge>
    matrix_storage<Vertice, Edge>::vertex_iterator end(const matrix_storage<Vertice, Edge> &graph) noexcept{
        return graph.end();
    }
}



