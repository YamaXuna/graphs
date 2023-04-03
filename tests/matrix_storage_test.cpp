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

template<typename Matrix>
void display_matrix(Matrix &&matrix){
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

void insertTest(){
    auto matrix = matrix_storage<double, std::string>();
    matrix.add("a");
    matrix.add("rr");

    matrix.add("a", "rr", 2);
    auto opt = matrix.edge("a", "rr");
    assert(opt.has_value());
    assert(*opt == 2);

    opt = matrix.edge("rr", "a");
    assert(!opt.has_value());

}


void getEdgeTest(){
    auto matrix = matrix_storage<double, int>();
    matrix.add(5);
    matrix.add(6);


    assert_not_throw([&matrix](){matrix.edge(5, 6);});
    assert_throw<VerticeDoesNotExistsError<int>>([&matrix](){
        matrix.edge(5, 8);
    });

};

void removeTest(){
    auto matrix = matrix_storage<double, int>();
    matrix.add(5);
    matrix.add(6);
    matrix.add(5, 6, 7);

    //display_matrix(matrix);
    matrix.remove(5);
    //display_matrix(matrix);
    assert(std::size(matrix.vertices()) == 1);
    assert(std::size(matrix.matrix()[0]) == 1);

}
template<typename T>
void foo(T &a)requires graph_storage<T>{
    display_matrix(a);
}

int main(){
    auto matrix = matrix_storage<double, std::string>();

    matrix.add("a");
    matrix.add("rr");

    matrix.add("a", "rr", 2);
    matrix.add("rr", "ab", 7);
    std::cout << matrix.edge("a", "rr").value_or(0) << '\n';
    //display_matrix(matrix);

    insertTest();
    getEdgeTest();
    removeTest();

}