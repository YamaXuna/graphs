#include "graph/matrix_storage.h"
#include <iostream>
#include <string>
#include <memory>
#include <cassert>
#include <functional>
#include "characters/Maid.h"
#include "test_utilyty.h"

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

void getEdgeFailureTest(){
    auto matrix = matrix_storage<double, int>();
    matrix.add(5);
    matrix.add(6);


    assertThrow<VerticeDoesNotExistsError<int>>([&matrix](){
        matrix.edge(5, 8);
    });

};

int main(){
    auto matrix = matrix_storage<double, std::string>();
    matrix.add("a");
    matrix.add("rr");

    matrix.add("a", "rr", 2);
    matrix.add("rr", "ab", 7);
    std::cout << matrix.edge("a", "rr").value_or(0) << '\n';
    display_matrix(matrix);

    insertTest();
    getEdgeFailureTest();

}