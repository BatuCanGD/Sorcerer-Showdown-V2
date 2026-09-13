#include "../header/Game.hpp"
#include "../header/Utilities/Input.hpp"
#include "../header/Creator.hpp"
#include "../header/CharacterType/Sorcerer.hpp"
#include "../header/Battlefield.hpp"

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

bool rungameloop(battlefield& bf, const playerchoices& pc) {
    for(const auto& c : bf.battlefield){
        std::println("{}", c->Name());
        if (const auto* crs = c->CanUseSorcery()){
            if (crs->Jujutsu().technique) {
                crs->Jujutsu().technique->PrintName();
                crs->Jujutsu().technique->PrintAbilities();
            }
        }
    }
    return false;
}

bool rungame()  {
    battlefield bf;
    bf.battlefield.push_back(Create::Gojo());

    const playerchoices pc{};

    while(rungameloop(bf, pc));
    return endgame();
}