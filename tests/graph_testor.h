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
        inline T operator()(const U &value){
            return T{value};
        }
    };
    template<typename T>
    struct factory<T, T>{
        inline T operator()(const T &value){
            return value;
        }
    };
    template<typename T>
    struct factory<std::unique_ptr<T>, T>{
        inline std::unique_ptr<T> operator()(const T &value){
            return std::make_unique<T>(value);
        }
    };
    template<typename T>
    struct factory<std::shared_ptr<T>, T>{
        inline std::shared_ptr<T> operator()(const T &value){
            return std::make_shared<T>(value);
        }
    };

    template <typename T>
    struct value_extractor {
        inline T operator()(const T& value) {
            return value;
        }
    };
    template <typename T>
    requires is_ptr<T>::value
    struct value_extractor<T> {
        inline decltype(auto) operator()(const T& value){
            return *value;
        }
    };
    template <typename T>
    inline decltype(auto) v_extract(const T& value) {
        return value_extractor<T>{}(value);
    }

    using namespace std::string_literals;
    template <template <typename, typename> typename Graph>
    class graph_testor {

        template<typename Vertice, typename Edge,
                typename Vf = factory<Vertice, Vertice>,
                typename Ef = factory<Edge, Edge>>
        requires graph<Graph<Vertice, Edge>>
        struct test_wrapper{
            using inner_vertice_t = std::remove_reference_t<decltype(v_extract(std::declval<Vertice>()))>;
            using inner_edge_t = std::remove_reference_t<decltype(v_extract(std::declval<Edge>()))>;
            Vf v_factory = Vf{};
            Ef e_factory = Ef{};
            std::array<inner_vertice_t , 5> m_vertices;
            std::array<inner_edge_t , 4> m_edges;

            test_wrapper()=delete;
            test_wrapper(std::initializer_list<inner_vertice_t> vertices, std::initializer_list<inner_edge_t> edges):
            m_vertices{}, m_edges{}{
                assert(vertices.size() >= 5);
                assert(edges.size() >= 4);
                int i = 0;
                for(const auto &v : vertices){
                    m_vertices[i] = v;
                    ++i;
                }
                i = 0;
                for(const auto &e : edges){
                    m_edges[i] = e;
                    ++i;
                }
            }

            void insert_test(){
                auto graph = Graph<Vertice, Edge>();
                graph.add(v_factory(m_vertices[0]));
                graph.add(v_factory(m_vertices[1]));

                graph.add(v_factory(m_vertices[0]), v_factory(m_vertices[1]), e_factory(m_edges[0]));
                decltype(auto) opt = graph.edge(v_factory(m_vertices[0]), v_factory(m_vertices[1]));
                assert(opt.has_value());
                assert(v_extract(*opt) == m_edges[0]);

                decltype(auto) opt2 = graph.edge(v_factory(m_vertices[1]), v_factory(m_vertices[0]));
                assert(!opt2.has_value());
            }
            void get_edge_test(){
                auto graph = Graph<Vertice, Edge>();
                graph.add(v_factory(m_vertices[0]));
                graph.add(v_factory(m_vertices[1]));
                graph.add(v_factory(m_vertices[0]), v_factory(m_vertices[1]), e_factory(m_edges[0]));

                assert_not_throw([this, &graph](){graph.edge(v_factory(m_vertices[0]), v_factory(m_vertices[1]));});
                assert_throw<VerticeDoesNotExistsError<Vertice>>([this, &graph](){
                    graph.edge(v_factory(m_vertices[2]), v_factory(m_vertices[3]));
                });
                decltype(auto) e = graph.edge(v_factory(m_vertices[0]), v_factory(m_vertices[1]));
                assert(v_extract(*e) == m_edges[0]);

            }
            void remove_vertice_test(){
                auto graph = Graph<Vertice, Edge>();
                graph.add(v_factory(m_vertices[0]));
                graph.add(v_factory(m_vertices[1]));
                graph.add(v_factory(m_vertices[2]));
                graph.add(v_factory(m_vertices[0]), v_factory(m_vertices[1]), e_factory(m_edges[0]));
                graph.remove(v_factory(m_vertices[1]));

                assert(std::distance(std::begin(graph), std::end(graph)) == 2);
            }

            void remove_edge_test(){
                auto graph = Graph<Vertice, Edge>();
                graph.add(v_factory(m_vertices[0]));
                graph.add(v_factory(m_vertices[1]));
                graph.add(v_factory(m_vertices[0]), v_factory(m_vertices[1]), e_factory(m_edges[0]));
                graph.add(v_factory(m_vertices[0]), v_factory(m_vertices[0]), e_factory(m_edges[1]));
                graph.add(v_factory(m_vertices[1]), v_factory(m_vertices[0]), e_factory(m_edges[2]));
                graph.add(v_factory(m_vertices[1]), v_factory(m_vertices[1]), e_factory(m_edges[3]));

                assert_not_throw([this, &graph]{
                    graph.remove(v_factory(m_vertices[0]), v_factory(m_vertices[1]));
                });

                assert(!graph.edge(v_factory(m_vertices[0]), v_factory(m_vertices[1])).has_value());

                assert_throw<VerticeDoesNotExistsError<Vertice>>([this, &graph]{
                    graph.remove(v_factory(m_vertices[2]), v_factory(m_vertices[1]));
                });
            }

            void iterator_test(){
                auto graph = Graph<Vertice, Edge>();
                graph.add(v_factory(m_vertices[0]));
                graph.add(v_factory(m_vertices[1]));

                graph.add(v_factory(m_vertices[2]), v_factory(m_vertices[1]), e_factory(m_edges[0]));

                assert(std::distance(begin(graph), end(graph)) == 3);
                graph.remove(v_factory(m_vertices[0]));
                assert(std::distance(begin(graph), end(graph)) == 2);
                graph.add(v_factory(m_vertices[3]));
                assert(std::distance(begin(graph), end(graph)) == 3);
            }

            void get_neighbours_test(){
                auto graph = Graph<Vertice, Edge>();

                graph.add(v_factory(m_vertices[0]), v_factory(m_vertices[1]), e_factory(m_edges[0]));
                graph.add(v_factory(m_vertices[2]), v_factory(m_vertices[1]), e_factory(m_edges[1]));
                graph.add(v_factory(m_vertices[2]), v_factory(m_vertices[0]), e_factory(m_edges[2]));
                graph.add(v_factory(m_vertices[0]), v_factory(m_vertices[2]), e_factory(m_edges[3]));
                graph.add(v_factory(m_vertices[0]), v_factory(m_vertices[0]), e_factory(m_edges[4]));

                assert(std::size(graph.neighbours(v_factory(m_vertices[0]))) == 3);

                auto graph2 = Graph<std::unique_ptr<int>, std::unique_ptr<std::string>>();
                graph2.add(std::make_unique<int>(5));

                graph2.add(std::make_unique<int>(1), std::make_unique<int>(2), std::make_unique<std::string>("okk"s));
                //graph2.remove(std::make_unique<int>(1));
                graph2.remove(std::make_unique<int>(5), std::make_unique<int>(2));
                graph2.edge(std::make_unique<int>(5), std::make_unique<int>(2));

                auto v = graph2.neighbours(std::make_unique<int>(5));
                assert(std::size(v) == 0);
            }
            // nothing to do here because it depend on edge type and args
            void bfs_test()requires graph<Graph<std::string , double>>{
                auto graph = Graph<std::string , double>();

                graph.add("a"s);
                graph.add("a"s, "b"s, 2.3);
                graph.add("c"s, "b"s, 5);
                graph.add("c"s, "a"s, 1.6);
                graph.add("a"s, "d"s, 2.7);
                graph.add("a"s, "a"s, 12);

                std::string buf;
                breadth_first_search(graph, [&buf](const auto &v){
                    buf += v;
                });
                assert(buf == "cbad");

                auto graph2 = Graph<std::unique_ptr<int>, std::unique_ptr<std::string>>();
                graph2.add(std::make_unique<int>(5));

                graph2.add(std::make_unique<int>(1), std::make_unique<int>(2), std::make_unique<std::string>("okk"s));
                graph2.remove(std::make_unique<int>(1));
                graph2.remove(std::make_unique<int>(5), std::make_unique<int>(2));
                graph2.edge(std::make_unique<int>(5), std::make_unique<int>(2));
                graph2.add(std::make_unique<int>(2), std::make_unique<int>(5), std::make_unique<std::string>("aa"s));

                std::string buffer;
                breadth_first_search(graph2, [&buffer](const auto &v){
                    buffer += std::to_string(*v);
                });
                assert(buffer == "25");
            }
            void dfs_test()requires graph<Graph<std::string , double>>{
                auto graph = Graph<std::string , double>();

                graph.add("a"s);
                graph.add("a"s, "b"s, 2.3);
                graph.add("c"s, "b"s, 5);
                graph.add("c"s, "a"s, 1.6);
                graph.add("a"s, "d"s, 2.7);
                graph.add("a"s, "a"s, 12);

                std::string buffer;
                auto it = std::find_if(begin(graph), end(graph), [](const auto &v){
                    return get_comparator<std::string>()(v, "d"s);
                });
                depth_first_search(graph, [&buffer](const auto &v){
                    buffer += v;
                }, it);
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
            test_wrapper<std::string, double>{{"a"s, "b"s, "c"s, "d"s, "e"s}, {1, 2, 75, 99, 128}}();

            test_wrapper<std::unique_ptr<std::string>, std::unique_ptr<double>,
                    factory<std::unique_ptr<std::string>, std::string>,
                            factory<std::unique_ptr<double>, double>>{{"a"s, "b"s, "c"s, "d"s, "e"s}, {1, 2, 75, 99, 128}}();
            test_wrapper<std::shared_ptr<std::string>, std::shared_ptr<double>,
                    factory<std::shared_ptr<std::string>, std::string>,
                    factory<std::shared_ptr<double>, double>>{{"a"s, "b"s, "c"s, "d"s, "e"s}, {1, 2, 75, 99, 128}}();
        }
    };

} // xuna
