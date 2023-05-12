//
// Created by USER on 22/04/2023.
//

#pragma once

#include "graph/common.h"
#include "test_utilyty.h"
#include <string>
#include <iostream>
namespace xuna {

    template<typename T, typename U>
    struct factory{
        inline T operator()(U &&value){
            return T{std::forward<U>(value)};
        }
    };
    template<typename T>
    struct factory<T, T>{
        inline T operator()(T &&value){
            return value;
        }
    };
    template<typename T>
    struct factory<std::unique_ptr<T>, T>{
        inline std::unique_ptr<T> operator()(T &&value){
            return std::make_unique<T>(value);
        }
    };


    using namespace std::string_literals;
    template <template <typename, typename> typename Graph>
    class graph_testor {

        template<typename Vertice, typename Edge,
                typename Vf = factory<Vertice, Vertice>,
                typename Ef = factory<Edge, Edge>>
        requires graph<Graph<Vertice, Edge>>
        struct test_wrapper{
            Vf v_factory = Vf{};
            Ef e_factory = Ef{};

            void insert_test(){
                auto matrix = Graph<Vertice, Edge>();
                matrix.add(v_factory("a"s));
                matrix.add(v_factory("rr"s));

                matrix.add(v_factory("a"s), v_factory("rr"s), e_factory(2));
                auto opt = matrix.edge(v_factory("a"s), v_factory("rr"s));
                assert(opt.has_value());
                assert(*opt == 2);

                opt = matrix.edge(v_factory("rr"s), v_factory("a"s));
                assert(!opt.has_value());
            }
            void get_edge_test()requires graph<Graph<int, double>>{
                auto matrix = Graph<int, double>();
                matrix.add(5);
                matrix.add(6);

                assert_not_throw([&matrix](){matrix.edge(5, 6);});
                assert_throw<VerticeDoesNotExistsError<int>>([&matrix](){
                    matrix.edge(5, 8);
                });

            }
            void remove_vertice_test()requires graph<Graph<int, double>>{
                auto matrix = Graph<int, double>();
                matrix.add(5);
                matrix.add(6);
                matrix.add(2);
                matrix.add(5, 6, 7);

                matrix.remove(6);
                assert(std::distance(std::begin(matrix), std::end(matrix)) == 2);
                assert(std::size(matrix.matrix()[0]) == 2);
            }

            void remove_edge_test()requires graph<Graph<std::string , std::string>>{
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

            void non_copyable_test()requires graph<Graph<std::unique_ptr<int>, std::unique_ptr<std::string>>>{
                //compiling test
                auto matrix = Graph<std::unique_ptr<int>, std::unique_ptr<std::string>>();
                matrix.add(std::make_unique<int>(5));

                matrix.add(std::make_unique<int>(1), std::make_unique<int>(2), std::make_unique<std::string>("okk"s));
                matrix.remove(std::make_unique<int>(1));

                matrix.remove(std::make_unique<int>(5), std::make_unique<int>(2));
                matrix.edge(std::make_unique<int>(5), std::make_unique<int>(2));
            }

            void iterator_test()requires graph<Graph<std::string , double>>{
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

            void get_neighbours_test()requires graph<Graph<std::string , double>>{
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
            void bfs_test()requires graph<Graph<std::string , double>>{
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
            void dfs_test()requires graph<Graph<std::string , double>>{
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
            void operator()() {
                insert_test();
                get_edge_test();
                remove_vertice_test();
                remove_edge_test();
                iterator_test();
                get_neighbours_test();
                bfs_test();
                dfs_test();

                std::cout << "success\n";
            }
        };


    public:

        void operator()(){
            test_wrapper<std::unique_ptr<std::string>, double, factory<std::unique_ptr<std::string>, std::string>>{}();
        }
    };

} // xuna
