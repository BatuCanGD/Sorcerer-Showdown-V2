#include "../header/Game.hpp"
#include "../header/Utilities/Input.hpp"
#include "../header/Battlefield.hpp"

#include <print>

struct playerchoices {
    bool x;
    bool y; // placeholder
    bool z;
};

bool endgame() {
    std::print("Continue Game?\n1 - Continue | 2 - Stop\n=> ");
    return get_input<int>() == 1;
}

bool rungameloop(battlefield& bf, const playerchoices& pc) {

    return true;
}

bool rungame()  {
    battlefield bf;

    const playerchoices pc{};

    while(rungameloop(bf, pc));
    return endgame();
}