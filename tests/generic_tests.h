//
// Created by USER on 22/04/2023.
//

#pragma once
#include "graph/common.h"
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
}