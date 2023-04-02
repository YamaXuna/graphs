//
// Created by USER on 02/04/2023.
//

#pragma once

#include <cassert>

// For free functions
template<typename Exception, typename F>
void assertThrow(F f) {
    try {
        f();
        assert(false && "Function did not throw expected exception");
    } catch (const Exception&) {
        // Expected exception was thrown
    } catch (...) {
        assert(false && "Function threw unexpected exception");
    }
}
