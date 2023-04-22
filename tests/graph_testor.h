//
// Created by USER on 22/04/2023.
//

#pragma once

#include "graph/common.h"
#include "generic_tests.h"

namespace xuna {

    template <template <typename, typename> typename Graph>
    class graph_testor {
    public:
        void operator()() {
            insert_test<Graph>();
            get_edge_test<Graph>();
        }
    };

} // xuna
