#include "../../../header/Systems/System/EffectSystem.hpp"
#include "../../../header/Systems/ResourceHandler.hpp"
#include "../../../header/CharacterType/Character.hpp"

std::vector<EffectStruct> EffectSystem::ApplyEffects(std::vector<StatusEffect>& ste, Character &c){
    std::vector<EffectStruct> st;
    for (const auto& x : ste){
        st.push_back(EffectSystem::ApplyEffectsType(x,c));
    }
    ResourceHandler::TickStatusEffects(ste);
    return st;
}

EffectStruct EffectSystem::ApplyEffectsType(const StatusEffect &ste, Character& c) {
    switch(ste.effect_type) {
        case EffectType::Increase:
            
            break;
        case EffectType::Decrease:

            break;
        case EffectType::Buff:

            break;
        case EffectType::Debuff:

            break;
        default: break;
    }
    return {};
}