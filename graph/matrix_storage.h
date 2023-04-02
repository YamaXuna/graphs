//
// Created by USER on 02/04/2023.
//

#pragma once

#include <vector>
#include <array>
#include <optional>
#include <iostream>
#include <unordered_map>

namespace xuna{

    template<typename Vertice>
    struct VerticeDoesNotExistsError : public std::runtime_error{
        explicit VerticeDoesNotExistsError(Vertice v): std::runtime_error{"Vertice isn't in the graph"}{}
    };

    /**
     *
     * @tparam Edge
     * @tparam Vertice
     */
    template<typename Edge, typename Vertice>
    class matrix_storage {
        using edge_t = std::optional<Edge>;

        std::unordered_map<Vertice, size_t> m_vertices;
        std::vector<std::vector<edge_t>> m_matrix;


        bool isVerticePresent(Vertice &v){
            return m_vertices.find(v) != end(m_vertices);
        }

    public:
        matrix_storage() = default;
        ~matrix_storage() = default;

        /**
         *
         * @param v the vertice to add
         */
        void add(Vertice &&v){
            m_vertices[std::forward<Vertice>(v)] = std::size(m_vertices);

            for(auto &line : m_matrix){
                line.resize(std::size(m_vertices));
            }
            m_matrix.push_back(std::vector<edge_t>(std::size(m_vertices)));
        }
        /**
         *
         * @param v the vertice to remove
         */
        void remove(Vertice &&v){

            m_vertices.remove(v);

            std::vector<std::vector<edge_t>> temp(std::size(m_matrix) - 1);
            std::swap(begin(m_matrix), std::advance(end(m_matrix), -1), begin(temp), end(temp));
            m_matrix = temp;

        }
        /**
         *
         * @return the adjacency matrix
         */
        std::vector<std::vector<edge_t>> &matrix()noexcept{
            return m_matrix;
        }

        /**
         *
         * @param source the source vertice
         * @param target the target vertice
         * @param edge the edge value
         */
        void add(Vertice &&source, Vertice &&target, Edge &&edge){
            if(!isVerticePresent(source))
                add(std::forward<Vertice>(source));
            auto pos_a = m_vertices[source];
            if(!isVerticePresent(target))
                add(std::forward<Vertice>(target));
            auto pos_b = m_vertices[target];

            m_matrix[pos_a][pos_b] = std::forward<Edge>(edge);
        }

        /**
         *
         * @param source the source vertice
         * @param target the target vertice
         */
        void remove(Vertice &&source, Vertice &&target){
            if(!isVerticePresent(source))
                throw VerticeDoesNotExistsError(source);
            auto pos_a = m_vertices[source];
            if(!isVerticePresent(target))
                throw VerticeDoesNotExistsError(target);
            auto pos_b = m_vertices[target];

            m_matrix[pos_a][pos_b].reset();
        }

        /**
         *
         * @param source the source vertice
         * @param target the target vertice
         * @return the optional containing the edge's value if it has been set
         * @throws if one vertex isn't present in the matrix
         */
        edge_t edge(Vertice &&source, Vertice &&target){

            if(!isVerticePresent(source))
                throw VerticeDoesNotExistsError(source);
            auto pos_a = m_vertices[source];
            if(!isVerticePresent(target))
                throw VerticeDoesNotExistsError(target);
            auto pos_b = m_vertices[target];

            return m_matrix[pos_a][pos_b];
        }

    };
}



