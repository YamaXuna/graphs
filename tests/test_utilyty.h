//
// Created by USER on 02/04/2023.
//

#pragma once

#include <cassert>

// For free functions
template<typename Exception, typename F>
void assert_throw(F f) {
    try {
        f();
        assert(false && "Function did not throw expected exception");
    } catch (const Exception&) {
        // Expected exception was thrown
    } catch (...) {
        assert(false && "Function threw unexpected exception");
    }
}

template<typename F>
void assert_not_throw(F f) {
    try {
        f();
    }catch (...) {
        assert(false && "Function threw unexpected exception");
    }
}