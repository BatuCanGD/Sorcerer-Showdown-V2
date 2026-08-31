#pragma once
#include "Utilities/Input.hpp"

#include <print>

inline bool endgame() {
    std::print("Continue Program?\n1 - Continue | 2 - Stop\n=> ");
    return get_input<int>() == 1;
}

inline bool rungame() {

    return endgame();
}
