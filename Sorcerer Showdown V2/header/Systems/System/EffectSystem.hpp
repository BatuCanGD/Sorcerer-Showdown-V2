#pragma once
#include "../../Stuff/StatusEffects.hpp"
#include <vector>

class Character;

struct EffectStruct final {
    double effect_severity;
    EffectForType type_of_effect;
    EffectType effect_type;
};

namespace EffectSystem {
    std::vector<EffectStruct> ApplyEffects(std::vector<StatusEffect> &status_effects, Character& c);
    EffectStruct ApplyEffectsType(const StatusEffect &ste, Character& c);
}