#include "../../../header/Systems/Mechanics/EffectSystem.hpp"
#include "../../../header/Systems/ResourceHandler.hpp"
#include "../../../header/Character.hpp"

std::pair<const std::vector<StatusEffect>&, const Character&> EffectSystem::ApplyEffects(Character &c){
    auto& s = c.state.status_effects;
    std::erase_if(s, [&](const auto& z){
        return z.turn_amount <= 0;
    });
    for (auto& x : s){
        EffectSystem::ApplyEffectsType(x,c);
        x.turn_amount--;
    }
    return {s, c};
}

void EffectSystem::ApplyEffectsType(const StatusEffect &ste, Character& c) {
    const double amount = ste.effect_type == EffectType::Decrease ? -ste.effect_amount : ste.effect_amount;

    switch(ste.effect_for_type){
        case EffectForType::CursedEnergy:
            if (auto& crs = c.curseUser){
                crs->ce_system.cursed_energy = std::min(crs->ce_system.cursed_energy + amount, crs->ce_system.max_cursed_energy);
            }
            break;
        case EffectForType::Durability:
            c.state.durability += amount;
            break;
        case EffectForType::Health:
            c.state.health = std::min(c.state.health + amount, c.state.max_health);
            break;
        case EffectForType::MaxOutput:
            if (auto& crs = c.curseUser){
                crs->output.max_output_potential += amount;
            }
            break;
        case EffectForType::Strength:
            c.state.strength += amount;
            break;
        default:
            break;
    }
}
    
std::string EffectSystem::GetEffectForTypeStr(EffectForType eft) {
    switch (eft) {
        case EffectForType::CursedEnergy:   return "Cursed Energy";
        case EffectForType::Health:         return "Health";
        case EffectForType::Durability:     return "Durability";
        case EffectForType::Strength:       return "Strength";
        case EffectForType::MaxOutput:      return "Max Output";
        default: return "Unknown";
    }
}