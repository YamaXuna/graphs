#include <iostream>
#include <string>
#include <memory>
#include <cassert>
#include <functional>
#include "test_utilyty.h"
#include "graph/graph.h"
#include "graph/algorithms.h"

using namespace xuna;
using namespace std::string_literals;

template<typename Matrix>
void display_matrix(Matrix &matrix){
    for(auto &e : matrix.matrix()){
        for(auto &f : e){
            if(f)
                std::cout << *f;
            else
                std::cout << '.';
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

    opt = matrix.edge("rr"s, "a"s);
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
    matrix.add(2);
    matrix.add(5, 6, 7);

    //display_matrix(matrix);
    matrix.remove(6);
    //display_matrix(matrix);
    assert(std::distance(std::begin(matrix), std::end(matrix)) == 2);
    assert(std::size(matrix.matrix()[0]) == 2);
    //throw 2;

}

void remove_edge_test(){
    auto matrix = matrix_storage<std::string , std::string>();
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

template<typename T>
void graph_concept_test(T &a)requires graph<T>{
    std::ignore = a;
}

void non_copyable_test(){
    //compiling test
    auto matrix = matrix_storage<std::unique_ptr<int>, std::unique_ptr<std::string>>();
    matrix.add(std::make_unique<int>(5));

    matrix.add(std::make_unique<int>(1), std::make_unique<int>(2), std::make_unique<std::string>("okk"s));
    matrix.remove(std::make_unique<int>(1));

    matrix.remove(std::make_unique<int>(5), std::make_unique<int>(2));
    matrix.edge(std::make_unique<int>(5), std::make_unique<int>(2));


}

void iterator_test(){
    auto matrix = matrix_storage<std::string , double>();

    matrix.add("a"s);
    matrix.add("a"s, "b"s, 2.3);
    matrix.add("c"s);

    assert(std::distance(begin(matrix), end(matrix)) == 3);
    matrix.remove("a"s);
    assert(std::distance(begin(matrix), end(matrix)) == 2);
    matrix.add("r"s);
    assert(std::distance(begin(matrix), end(matrix)) == 3);
}

void get_neighbours_test(){
    auto matrix = matrix_storage<std::string , double>();

    matrix.add("a"s);
    matrix.add("a"s, "b"s, 2.3);
    matrix.add("c"s, "b"s, 5);
    matrix.add("c"s, "a"s, 1.6);
    matrix.add("a"s, "d"s, 2.7);
    matrix.add("a"s, "a"s, 12);

    assert(std::size(matrix.neighbours("a"s)) == 3);


    auto matrix2 = matrix_storage<std::unique_ptr<int>, std::unique_ptr<std::string>>();
    matrix2.add(std::make_unique<int>(5));

    matrix2.add(std::make_unique<int>(1), std::make_unique<int>(2), std::make_unique<std::string>("okk"s));
    //matrix2.remove(std::make_unique<int>(1));
    matrix2.remove(std::make_unique<int>(5), std::make_unique<int>(2));
    matrix2.edge(std::make_unique<int>(5), std::make_unique<int>(2));

    auto v = matrix2.neighbours(std::make_unique<int>(5));
    assert(std::size(v) == 0);
}

void bfs_test(){
    auto matrix = graph_wrapper<matrix_storage<std::string , double>>();

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

    auto matrix2 = matrix_storage<std::unique_ptr<int>, std::unique_ptr<std::string>>();
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

void dfs_test(){
    auto matrix = matrix_storage<std::string , double>();

    matrix.add("a"s);
    matrix.add("a"s, "b"s, 2.3);
    matrix.add("c"s, "b"s, 5);
    matrix.add("c"s, "a"s, 1.6);
    matrix.add("a"s, "d"s, 2.7);
    matrix.add("a"s, "a"s, 12);

    std::string buffer;

    depth_first_search(matrix, [&buffer](const auto &v){
        buffer += v;
    });
    //assert(buffer == "");
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
    non_copyable_test(); //not a unit test
    iterator_test();
    get_neighbours_test();

    bfs_test();
    dfs_test();

    std::cout << "success\n";

}