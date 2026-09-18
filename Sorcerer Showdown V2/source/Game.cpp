#include "../header/Game.hpp"
#include "../header/Logger.hpp"
#include "../header/Creator.hpp"
#include "../header/Battlefield.hpp"
#include "../header/Utilities/Input.hpp"
#include "../header/CharacterType/CurseUser.hpp"

#include <print>
#include <vector>

struct playerchoices {
    bool x;
    bool y; // placeholder
    bool z;
};

bool endgame() {
    std::print("Continue Game?\n1 - Continue | 2 - Stop\n=> ");
    return get_input<int>() == 1;
}

bool rungameloop(Battlefield& bf, const playerchoices& pc) {
    for(const auto& c : bf.battlefield){
       Log::CharacterInfo(*c);
    }
    return false;
}

bool rungame()  {
    Battlefield bf;
    bf.battlefield.push_back(Create::Gojo());

    const playerchoices pc{};

    while(rungameloop(bf, pc));
    return endgame();
}