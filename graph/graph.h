//
// Created by USER on 01/04/2023.
//

#pragma once

#include "matrix_storage.h"

namespace xuna {

    template<typename Edge, typename Vertice>
    class graph {
        using storage_t = matrix_storage<Edge, Vertice>;

        storage_t storage;

    public:
        graph()=default;
        ~graph()=default;

        /**
         *
         * @param v the vertice to add
         */
        void add(Vertice &&v){
            storage.add(std::forward<Vertice>(v));
        }
        /**
         *
         * @param source the source vertice
         * @param target the target vertice
         * @param edge the edge value
         */
        void add(Vertice &&source, Vertice &&target, Edge &&edge){
            storage.add(std::forward<Vertice>(source), std::forward<Vertice>(target), std::forward<Edge>(edge));
        }
        /**
         *
         * @param v the vertice to remove
         */
        void remove(Vertice &&v){
            storage.remove(std::forward<Vertice>(v));
        }
        /**
        *
        * @param source the source vertice
        * @param target the target vertice
        */
        void remove(Vertice &&source, Vertice &&target){
            storage.remove(std::forward<Vertice>(source), std::forward<Vertice>(target));
        }
        /**
         *
         * @param source the source vertice
         * @param target the target vertice
         * @return the optional containing the edge's value if it has been set
         * @throws if one vertice isn't present in the matrix
         */
        storage_t::edge_t edge(Vertice &&source, Vertice &&target)const{
            storage.edge(std::forward<Vertice>(source), std::forward<Vertice>(target));
        }
    };

} // xuna

