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
        using edges = std::optional<Edge>;

        std::unordered_map<Vertice, size_t> m_vertices;
        std::vector<std::vector<edges>> m_matrix;


        bool isVertice_present(Vertice &v)const {
            return m_vertices.find(v) != end(m_vertices);
        }

    public:
        using vertice_t = Vertice;
        using edge_t = Edge;

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
            m_matrix.push_back(std::vector<edges>(std::size(m_vertices)));
        }

        /**
         *
         * @param source the source vertice
         * @param target the target vertice
         * @param edge the edge value
         */
        void add(Vertice &&source, Vertice &&target, Edge &&edge){
            if(!isVertice_present(source))
                add(std::forward<Vertice>(source));
            auto pos_a = m_vertices[source];
            if(!isVertice_present(target))
                add(std::forward<Vertice>(target));
            auto pos_b = m_vertices[target];

            m_matrix[pos_a][pos_b] = std::forward<Edge>(edge);
        }

        /**
         *
         * @param v the vertice to remove
         */
        void remove(Vertice &&v){

            m_vertices.remove(v);

            std::vector<std::vector<edges>> temp(std::size(m_matrix) - 1);
            std::swap(begin(m_matrix), std::advance(end(m_matrix), -1), begin(temp), end(temp));
            m_matrix = temp;

        }
        /**
        *
        * @param source the source vertice
        * @param target the target vertice
        */
        void remove(Vertice &&source, Vertice &&target){
            if(!isVertice_present(source))
                throw VerticeDoesNotExistsError(source);
            auto pos_a = m_vertices[source];
            if(!isVertice_present(target))
                throw VerticeDoesNotExistsError(target);
            auto pos_b = m_vertices[target];

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
        std::vector<std::reference_wrapper<Vertice>> vertices()noexcept{
            std::vector<std::reference_wrapper<Vertice>> vect;
            for(auto &e : m_vertices){
                vect.push_back(e);
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
        edges edge(Vertice &&source, Vertice &&target)const{
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



