#include "../../header/Creator.hpp"
#include "../../header/General/CursedTool.hpp"

CursedTool Create::Tool::InvertedSpearOfHeaven() {
    CursedTool c{};
    const EntityInfo id = {"Inverted Spear Of Heaven", "\x1b[38;5;179m", "A Cursed Tool that negates Techniques"};
    constexpr double damage = 75.0;
    const auto at_type = DamageType::BypassTech;
    c.identity = id;
    c.damage = damage;
    c.damage_type = at_type;
    c.given_effect = Create::Effect::Bleed();
    return c;
}
CursedTool Create::Tool::SplitSoulKatana() {
    CursedTool c{};
    const EntityInfo id = {"Split Soul Katana", "\x1b[38;5;4m", "A Cursed Tool that negates Durability and cuts the soul"};
    constexpr double damage = 80.0;
    const auto at_type = DamageType::BypassRein;
    c.identity = id;
    c.damage = damage;
    c.damage_type = at_type;
    c.given_effect = Create::Effect::Bleed();
    return c;
}
CursedTool Create::Tool::PlayfulCloud() {
    CursedTool c{};
    const EntityInfo id = {"Playful Cloud", "\x1b[38;5;9m", "A Cursed Tool that's strength increases based on the bearers strength"};
    constexpr double damage = 70.0;
    const auto at_type = DamageType::Normal;
    c.identity = id;
    c.damage = damage;
    c.damage_type = at_type;
    c.given_effect = Create::Effect::Bleed();
    return c;
}