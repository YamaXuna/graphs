//
// Created by USER on 22/04/2023.
//

#pragma once
#include "graph/graph.h"
#include "test_utilyty.h"
#include <string>
#include <iostream>
namespace xuna{

    using namespace std::string_literals;

    template<template <typename, typename>  typename Graph> requires graph<Graph<std::string, double>>
    void insert_test(){
        auto matrix = Graph<std::string, double>();
        matrix.add("a");
        matrix.add("rr");

        matrix.add("a"s, "rr"s, 2);
        auto opt = matrix.edge("a"s, "rr"s);
        assert(opt.has_value());
        assert(*opt == 2);

        opt = matrix.edge("rr"s, "a"s);
        assert(!opt.has_value());


    }

    template<template <typename, typename>  typename Graph> requires graph<Graph<int, double>>
    void get_edge_test(){
        auto matrix = Graph<int, double>();
        matrix.add(5);
        matrix.add(6);

        assert_not_throw([&matrix](){matrix.edge(5, 6);});
        assert_throw<VerticeDoesNotExistsError<int>>([&matrix](){
            matrix.edge(5, 8);
        });

    }
    template<template <typename, typename>  typename Graph> requires graph<Graph<int, double>>
    void remove_vertice_test(){
        auto matrix = Graph<int, double>();
        matrix.add(5);
        matrix.add(6);
        matrix.add(2);
        matrix.add(5, 6, 7);

        matrix.remove(6);
        assert(std::distance(std::begin(matrix), std::end(matrix)) == 2);
        assert(std::size(matrix.matrix()[0]) == 2);
    }

    template<template <typename, typename>  typename Graph> requires graph<Graph<std::string , std::string>>
    void remove_edge_test(){
        auto matrix = Graph<std::string , std::string>();
        matrix.add("a");
        matrix.add("rr");

        matrix.add("a", "rr", "test");
        matrix.add("a", "a", "p");
        matrix.add("rr", "a", "ttt");
        matrix.add("rr", "rr", "p");

        assert_not_throw([&matrix]{
            matrix.remove("a", "rr");
        });

        assert(!matrix.edge("a", "rr").has_value());

        assert_throw<VerticeDoesNotExistsError<std::string>>([&matrix]{
            matrix.remove("agh", "rr");
        });
    }

    template<template <typename, typename>  typename Graph>
    requires graph<Graph<std::unique_ptr<int>, std::unique_ptr<std::string>>>
    void non_copyable_test(){
        //compiling test
        auto matrix = Graph<std::unique_ptr<int>, std::unique_ptr<std::string>>();
        matrix.add(std::make_unique<int>(5));

        matrix.add(std::make_unique<int>(1), std::make_unique<int>(2), std::make_unique<std::string>("okk"s));
        matrix.remove(std::make_unique<int>(1));

        matrix.remove(std::make_unique<int>(5), std::make_unique<int>(2));
        matrix.edge(std::make_unique<int>(5), std::make_unique<int>(2));
    }

    template<template <typename, typename>  typename Graph> requires graph<Graph<std::string , double>>
    void iterator_test(){
        auto matrix = Graph<std::string , double>();

        matrix.add("a"s);
        matrix.add("a"s, "b"s, 2.3);
        matrix.add("c"s);

        assert(std::distance(begin(matrix), end(matrix)) == 3);
        matrix.remove("a"s);
        assert(std::distance(begin(matrix), end(matrix)) == 2);
        matrix.add("r"s);
        assert(std::distance(begin(matrix), end(matrix)) == 3);
    }

    template<template <typename, typename>  typename Graph> requires graph<Graph<std::string , double>>
    void get_neighbours_test(){
        auto matrix = Graph<std::string , double>();

        matrix.add("a"s);
        matrix.add("a"s, "b"s, 2.3);
        matrix.add("c"s, "b"s, 5);
        matrix.add("c"s, "a"s, 1.6);
        matrix.add("a"s, "d"s, 2.7);
        matrix.add("a"s, "a"s, 12);

        assert(std::size(matrix.neighbours("a"s)) == 3);


        auto matrix2 = Graph<std::unique_ptr<int>, std::unique_ptr<std::string>>();
        matrix2.add(std::make_unique<int>(5));

        matrix2.add(std::make_unique<int>(1), std::make_unique<int>(2), std::make_unique<std::string>("okk"s));
        //matrix2.remove(std::make_unique<int>(1));
        matrix2.remove(std::make_unique<int>(5), std::make_unique<int>(2));
        matrix2.edge(std::make_unique<int>(5), std::make_unique<int>(2));

        auto v = matrix2.neighbours(std::make_unique<int>(5));
        assert(std::size(v) == 0);
    }

    template<template <typename, typename>  typename Graph> requires graph<Graph<std::string , double>>
    void bfs_test(){
        auto matrix = Graph<std::string , double>();

        matrix.add("a"s);
        matrix.add("a"s, "b"s, 2.3);
        matrix.add("c"s, "b"s, 5);
        matrix.add("c"s, "a"s, 1.6);
        matrix.add("a"s, "d"s, 2.7);
        matrix.add("a"s, "a"s, 12);

        std::string buf;
        breadth_first_search(matrix, [&buf](const auto &v){
            buf += v;
        });
        assert(buf == "cbad");

        auto matrix2 = Graph<std::unique_ptr<int>, std::unique_ptr<std::string>>();
        matrix2.add(std::make_unique<int>(5));

        matrix2.add(std::make_unique<int>(1), std::make_unique<int>(2), std::make_unique<std::string>("okk"s));
        matrix2.remove(std::make_unique<int>(1));
        matrix2.remove(std::make_unique<int>(5), std::make_unique<int>(2));
        matrix2.edge(std::make_unique<int>(5), std::make_unique<int>(2));
        matrix2.add(std::make_unique<int>(2), std::make_unique<int>(5), std::make_unique<std::string>("aa"s));

        std::string buffer;
        breadth_first_search(matrix2, [&buffer](const auto &v){
            buffer += std::to_string(*v);
        });
        assert(buffer == "25");
    }

    template<template <typename, typename>  typename Graph> requires graph<Graph<std::string , double>>
    void dfs_test(){
        auto matrix = Graph<std::string , double>();

        matrix.add("a"s);
        matrix.add("a"s, "b"s, 2.3);
        matrix.add("c"s, "b"s, 5);
        matrix.add("c"s, "a"s, 1.6);
        matrix.add("a"s, "d"s, 2.7);
        matrix.add("a"s, "a"s, 12);

        std::string buffer;
        auto it = std::find_if(begin(matrix), end(matrix), [](const auto &v){
            return get_comparator<std::string>()(v, "d"s);
        });
        depth_first_search(matrix, [&buffer](const auto &v){
            buffer += v;
        }, it);
        //std::cout << buffer;
        assert(buffer == "d");
    }

}