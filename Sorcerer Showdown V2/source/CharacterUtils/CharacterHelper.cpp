#include "../../header/CharacterUtils/CharacterHelper.hpp"
#include "../../header/CharacterType/CurseUser.hpp"


double Helper::DealWithDamage(Character& c, enumtype::DamageType type, double amount){
    if ([[maybe_unused]] auto crs = dynamic_cast<CurseUser*>(&c)){
        // add technique stuff and reinforcement checking here!!!
    }
    return amount;
}