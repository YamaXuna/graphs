//
// Created by USER on 05/04/2023.
//

#pragma once

#include <type_traits>
#include <memory>
#include <optional>
#include <vector>
#include <functional>
//#include "graph_wrapper.h"

namespace xuna {
    template<typename T>
    struct is_smart_pointer : std::false_type {};

    template<typename T>
    struct is_smart_pointer<std::unique_ptr<T>> : std::true_type {};

    template<typename T>
    struct is_smart_pointer<std::shared_ptr<T>> : std::true_type {};

    /**
     *  check if T is a pointer or a smart pointer
     * @tparam T
     */
    template<typename T>
    struct is_ptr : std::bool_constant<is_smart_pointer<T>::value || std::is_pointer_v<T>> {};

    /**
     * helper
     */
    template<typename T>
    using is_ptr_v = is_ptr<T>::value;

    // Overload of begin for Graphs
    template <typename V, typename E, template<typename, typename> class Graph>
    auto begin(Graph<V, E>& g) -> decltype(g.begin()) {
        return g.begin();
    }

    // Overload of begin for const Graphs
    template <typename V, typename E, template<typename, typename> class Graph>
    auto begin(const Graph<V, E>& g) -> decltype(g.cbegin()) {
        return g.cbegin();
    }

    template<typename Graph>
    concept elementary_graph = requires(Graph g, typename Graph::vertice_t v, typename Graph::edge_t e){
        g.add(typename Graph::vertice_t{});
        g.add(typename Graph::vertice_t{}, typename Graph::vertice_t{}, typename Graph::edge_t{});
        {g.edge(typename Graph::vertice_t{}, typename Graph::vertice_t{})} ->
        std::same_as<std::optional<std::reference_wrapper<const typename Graph::edge_t>>>;
        g.remove(typename Graph::vertice_t{});
        g.remove(typename Graph::vertice_t{}, typename Graph::vertice_t{});
    };

/**
*
* @tparam Graph the graph, it must have type traits for both vertices and edges
*/
    template<typename Graph>
    concept graph = requires (Graph g, typename Graph::vertice_t v, typename Graph::edge_t e,
            typename Graph::vertex_iterator) {
        requires elementary_graph<Graph>;
        { g.begin() } -> std::same_as<typename Graph::vertex_iterator>;
        { g.end() } -> std::same_as<typename Graph::vertex_iterator>;
        { g.neighbours(typename Graph::vertice_t{}) } ->
            std::same_as<std::vector<std::pair<
            std::reference_wrapper<const typename Graph::vertice_t>, std::reference_wrapper<const typename Graph::edge_t>
            >
            >>;
    };

/**
 *
 * @tparam Vertice
 */
    template<typename Vertice>
    struct VerticeDoesNotExistsError : public std::runtime_error {
        explicit VerticeDoesNotExistsError(const Vertice &v) : std::runtime_error{"Vertice isn't in the graph"} {}
    };


    template<typename T>
    using comparator_t = std::function<bool(const T&, const T&)>;

    template<typename T>
    comparator_t<T> get_comparator(){
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

    /**
    *  function from boost
    * @tparam T
    * @param seed
    * @param v
    */
    template <class T>
    inline void hash_combine(std::size_t & seed, const T & v)
    {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    /**
     * struct to specialize to compute hashes on pairs
     * @tparam T
     */
    template<typename T>
    struct pair_hash;
    /**
     *
     * @tparam S
     * @tparam T
     */
    template<typename S, typename T> struct pair_hash<std::pair<S, T>>
    {
        inline std::size_t operator()(const std::pair<S, T> & v) const
        {
            std::size_t seed = 0;
            hash_combine(seed, v.first);
            hash_combine(seed, v.second);
            return seed;
        }
    };

}