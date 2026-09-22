#include "../../../header/Systems/System/EffectSystem.hpp"
#include "../../../header/Systems/ResourceHandler.hpp"
#include "../../../header/CharacterType/CurseUser.hpp"

std::pair<const std::vector<StatusEffect>&, const Character&> EffectSystem::ApplyEffects(std::vector<StatusEffect>& ste, Character &c){
    ResourceHandler::TickStatusEffects(ste);
    for (const auto& x : ste){
        EffectSystem::ApplyEffectsType(x,c);
    }
    return {ste, c};
}

void EffectSystem::ApplyEffectsType(const StatusEffect &ste, Character& c) {
    OpType tp{OpType::Subtract};
    bool subtract{false};
    switch(ste.effect_type) {
        case EffectType::Increase:
            tp = OpType::Add;
            break;
        case EffectType::Decrease:
            tp = OpType::Subtract;
            subtract = true;
            break;
        default: 
            break;
    }
    switch(ste.effect_for_type){
        case EffectForType::CursedEnergy:
            if (auto* crs = c.CanUseSorcery()){
                crs->CursedEnergy(tp, ste.effect_amount);
            }
            break;
        case EffectForType::Durability:
            if (subtract) {
                c.State().durability -= ste.effect_amount;
            } else {
                c.State().durability += ste.effect_amount;
            }
            break;
        case EffectForType::Health:
            c.Health(tp, ste.effect_amount);
            break;
        case EffectForType::MaxOutput:
            if (auto* crs = c.CanUseSorcery()){
                if (subtract){
                    crs->Output().max_output_potential -= ste.effect_amount;
                }else {
                    crs->Output().max_output_potential += ste.effect_amount;
                }
            }
            break;
        case EffectForType::Strength:
            if (subtract){
                c.State().strength -= ste.effect_amount;
            }else {
                c.State().strength += ste.effect_amount;
            }
            break;
        default:
            break;
    }
}

std::string GetEffectForTypeStr(EffectForType eft) {
    switch (eft) {
        case EffectForType::CursedEnergy:   return "Cursed Energy";
        case EffectForType::Health:         return "Health";
        case EffectForType::Durability:     return "Durability";
        case EffectForType::Strength:       return "Strength";
        case EffectForType::MaxOutput:      return "Max Output";
        default: return "Unknown";
    }
}