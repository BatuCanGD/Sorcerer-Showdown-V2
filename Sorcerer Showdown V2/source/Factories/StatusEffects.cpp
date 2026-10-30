#include "../../header/Creator.hpp"
#include "../../header/General/StatusEffects.hpp"
#include <format>

StatusEffect Create::Effect::Bleed() {
    StatusEffect c{};

    constexpr double severity_amount = 25.0;
    constexpr int effect_turn_amount = 3;

    const EntityInfo id = { "Bleeding", "\x1b[38;5;9m", std::format("Drains {:.1f}HP per turn", severity_amount)};

    c.id = id;
    c.effect_amount = severity_amount;
    c.effect_type = EffectType::Decrease;
    c.effect_for_type = EffectForType::Health;
    c.turn_amount = effect_turn_amount;

    return c;
}