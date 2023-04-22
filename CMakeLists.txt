cmake_minimum_required(VERSION 3.24)
project(graphs)

set(CMAKE_CXX_STANDARD 23)

include_directories(.)
include_directories(tests/characters)

add_executable(graphs
        graph/graph_wrapper.h graph/matrix_storage.h tests/matrix_storage_test.cpp tests/test_utilyty.h graph/common.h graph/graph.h graph/algorithms.h tests/algorithms_tests.cpp tests/graph_testor.h tests/generic_tests.h tests/tests.cpp)
