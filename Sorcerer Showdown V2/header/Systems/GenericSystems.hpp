#pragma once
#include "../Enums.hpp"

class Character;

namespace GenericSystems {
    bool BypassesTech(globalums::DamageType dt);
    bool BypassesReinforcement(globalums::DamageType dt);
    bool HPMoreThanMax(Character& c, double p);
}