#include "../../header/Creator.hpp"
#include "../../header/Sorcery/Shikigami.hpp"

Shikigami Create::Shk::Mahoraga() {
    Shikigami c{};
    const EntityInfo id = { "Mahoraga", "\x1b[38;5;220m", "Shikigami that can adapt to anything and everything"};

    c.id = id;
    c.cost = 300.0;
    c.stats.max_health = 800.0;
    c.stats.health = c.stats.max_health;
    c.stats.durability = 150.0;
    c.stats.regen_speed = 50.0;
    c.active_ability = ActiveAbility::TechniqueBypass;
    c.support_type = SupportType::Defense;

    return c;
}
Shikigami Create::Shk::Agito() {
    Shikigami c{};
    const EntityInfo id = { "Merged Beast Agito", "\x1b[38;5;202m", ""};

    c.id = id;
    c.cost = 300.0;
    c.stats.max_health = 250.0;
    c.stats.health = c.stats.max_health;
    c.stats.durability = 200.0;
    c.stats.regen_speed = 35.0;
    c.support_type = SupportType::Output;

    return c;
}
Shikigami Create::Shk::EmberInsects() {
    Shikigami c{};
    const EntityInfo id = { "Ember Insects", "\x1b[38;5;202m", "Shikigami that attack in swarms and are explosive"};

    c.id = id;
    c.support_type = SupportType::CursedEnergyRegen;
    c.cost = 100.0;
    c.stats.max_health = 50.0;
    c.stats.health = c.stats.max_health;
    c.stats.regen_speed = 50.0;
    c.stats.durability = 25.0;

    return c;
}