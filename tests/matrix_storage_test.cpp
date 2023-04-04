#include <iostream>
#include <string>
#include <memory>
#include <cassert>
#include <functional>
#include "characters/Maid.h"
#include "test_utilyty.h"
#include "graph/matrix_storage.h"
#include "graph/graph.h"

using namespace xuna;
using namespace std::string_literals;

template<typename Matrix>
void display_matrix(Matrix &matrix){
    for(auto &e : matrix.matrix()){
        for(auto &f : e){
            if(f)
                std::cout << *f;
            else
                std::cout << ".";
            std::cout << ' ';
        }
        std::cout << '\n';
    }
}

void insert_test(){
    auto matrix = matrix_storage<std::string, double>();
    matrix.add("a");
    matrix.add("rr");

    matrix.add("a"s, "rr"s, 2);
    auto opt = matrix.edge("a"s, "rr"s);
    assert(opt.has_value());
    assert(*opt == 2);

    opt = matrix.edge("rr", "a");
    assert(!opt.has_value());

}


void get_edge_test(){
    auto matrix = matrix_storage<int, double>();
    matrix.add(5);
    matrix.add(6);


    assert_not_throw([&matrix](){matrix.edge(5, 6);});
    assert_throw<VerticeDoesNotExistsError<int>>([&matrix](){
        matrix.edge(5, 8);
    });

}

void remove_vertice_test(){
    auto matrix = matrix_storage<int, double>();
    matrix.add(5);
    matrix.add(6);
    matrix.add(5, 6, 7);

    //display_matrix(matrix);
    matrix.remove(5);
    //display_matrix(matrix);
    assert(std::size(matrix.vertices()) == 1);
    assert(std::size(matrix.matrix()[0]) == 1);

}

void remove_edge_test(){
    auto matrix = matrix_storage<std::string , std::string>();
    matrix.add("a");
    matrix.add("rr");

    matrix.add("a", "rr", "test");

    assert_not_throw([&matrix]{
        matrix.remove("a", "rr");
    });

    assert_throw<VerticeDoesNotExistsError<std::string>>([&matrix]{
        matrix.remove("agh", "rr");
    });
}

template<typename T>
void graph_concept_test(T &a)requires graph_storage<T>{
    std::ignore = a;
}

void polymorphism_test(){
    auto matrix = matrix_storage<std::shared_ptr<Character>, std::string>();
    std::shared_ptr<Character> c{std::make_shared<Character>("C1", 24, Character::Gender::WOMAN, "")};
    std::shared_ptr<Maid> m{std::make_shared<Maid>("M1", 19, *c)};
    matrix.add(c);
    matrix.add(m);
    matrix.add(c, m, "mistress");
    matrix.add(m, c, "maid");
    display_matrix(matrix);
}

int main(){
    auto matrix = matrix_storage<std::string, double>();

    matrix.add("a");
    matrix.add("rr");

    matrix.add("a", "rr", 2);
    matrix.add("rr", "ab", 7);
    //std::cout << matrix.edge("a", "rr").value_or(0) << '\n';
    //display_matrix(matrix);

    insert_test();
    get_edge_test();
    remove_vertice_test();
    remove_edge_test();
    graph_concept_test(matrix);
    //polymorphism_test(); //not a unit test

}