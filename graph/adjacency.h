//
// Created by USER on 01/02/2024.
//

#pragma once

#include "common.h"
#include <unordered_map>
#include <unordered_set>

namespace xuna {

    /**
     *
     * @tparam Vertice
     * @tparam Edge
     */
    template<typename Vertice, typename Edge>
    class adjacency_list {
        using pair_t = std::pair<Vertice, Edge>;
        using set_t = std::unordered_set<pair_t, pair_hash<pair_t>>;
        using map_t = std::unordered_map<Vertice, set_t>;
        map_t m_list;

        inline decltype(begin(std::declval<set_t>()))
            find_edge(const Vertice &source, const Vertice &target)const{
            using std::begin, std::end;
            decltype(auto) set = m_list.at(source);
                return std::find_if(begin(set), end(set), [&target](auto &p){
                    return p.first == target;
                });
        }

    public:
        class vertex_iterator {
            using iterator = typename map_t::const_iterator;
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
                return m_it != other.m_it;
            }
        };

        using edge_t = Edge;
        using vertice_t = Vertice;


        adjacency_list() = default;
        adjacency_list(const adjacency_list &other) = default;
        adjacency_list(adjacency_list &&other) noexcept = default;


        /**
         * 
         * @tparam V
         * @param vertice the vertice to add
         */
        template<typename V>
        void add(V &&vertice){
            if(m_list.contains(vertice))
                return;
            m_list[std::forward<V>(vertice)] = set_t{};
        }


        /**
         * @tparam V
         * @tparam V2
         * @tparam E
         * @param source the source vertice
         * @param target the target vertice
         * @param edge the edge value
         */
        template<typename V, typename V2, typename E>
        void add(V &&source, V2 &&target, E &&edge){
            if(!m_list.contains(source))
                add(source);
            if(!m_list.contains(target))
                add(target);

            m_list.at(source).insert(std::pair<Vertice, Edge>{target, std::forward<E>(edge)});
        }

        /**
         *
         * @param v the vertice to remove
         */
        void remove(const Vertice &v){
            if(!m_list.contains(v))
                throw VerticeDoesNotExistsError(v);
            m_list.erase(v);
        }
        /**
        *
        * @param source the source vertice
        * @param target the target vertice
        */
        void remove(const Vertice &source, const Vertice &target){
            using std::begin, std::end;
            if(!m_list.contains(source))
                throw VerticeDoesNotExistsError(source);
            if(!m_list.contains(target))
                throw VerticeDoesNotExistsError(target);

            set_t &set = m_list.at(source);
            decltype(auto) it = find_edge(source, target);
            if(it != end(set))
                set.erase(it);
        }

        /**
         *
         * @return the vertices
         */

        vertex_iterator begin()const noexcept{
            using std::begin;
            return vertex_iterator(begin(m_list));
        }
        vertex_iterator end() noexcept{
            using std::end;
            return vertex_iterator(end(m_list));
        }

        /**
         *
         * @param source the source vertice
         * @param target the target vertice
         * @return the optional containing the edge's value if it has been set
         * @throws if one vertice isn't present in the matrix
         */
        std::optional<std::reference_wrapper<const Edge>> edge(
                const Vertice &source, const Vertice &target)const{
            using std::end;
            if(!m_list.contains(source))
                throw VerticeDoesNotExistsError(source);
            if(!m_list.contains(target))
                throw VerticeDoesNotExistsError(target);

            decltype(auto) it{find_edge(source, target)};
            static const std::optional<Edge> emptyEdge;
            if(it == end(m_list.at(source)))
                return std::nullopt;
            return std::optional(std::cref(it->second));

        }

    };

} // xuna

