#pragma once
#include "../../Stuff/StatusEffects.hpp"
#include <utility>
#include <vector>

class Character;

namespace EffectSystem {
    std::pair<const std::vector<StatusEffect>&, const Character&> ApplyEffects(std::vector<StatusEffect> &status_effects, Character& c);
    void ApplyEffectsType(const StatusEffect &ste, Character& c);
    std::string GetEffectForTypeStr(EffectForType eft);
}