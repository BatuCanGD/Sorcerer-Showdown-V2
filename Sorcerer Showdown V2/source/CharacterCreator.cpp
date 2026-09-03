#include "../header/CharacterCreator.hpp"
#include "../header/CharacterEditor.hpp"
#include "../header/Utilities/Random.hpp"
#include "../header/CharacterType/Character.hpp"
#include "../header/CharacterType/CurseUser.hpp"
#include "../header/CharacterType/Sorcerer.hpp"

#include <memory>

// character
std::unique_ptr<Character> Create::TranfiguredHuman() {
    auto c = std::make_unique<Character>();

    constexpr const char* name = "Transfigured Human";
    constexpr const char* color = "\x1b[91m";
    const double health = get_random<double>(1.0, 100.0);
    const double strength = get_random<double>(1.0, 100.0);
    const double durability = get_random<double>(1.0, 100.0);

    Editor::SetName(*c, name);
    Editor::SetColor(*c, color);
    Editor::SetHealth(*c, health);
    Editor::SetDurability(*c, durability);
    Editor::SetStrength(*c, strength);

    return c;
}
// curse user
std::unique_ptr<CurseUser> Create::Mahito() {
    auto c = std::make_unique<CurseUser>();

    constexpr const char* name = "Mahito";
    constexpr const char* color = "\x1b[91m";
    constexpr double health = 550.0;
    constexpr double cursed_energy = 4000.0;
    constexpr double strength = 115.0;
    constexpr double durability = 75.0;
    constexpr auto ce_efficiency = CurseUser::CEfficiency::Stable;

    Editor::SetName(*c, name);
    Editor::SetColor(*c, color);
    Editor::SetHealth(*c, health);
    Editor::SetDurability(*c, durability);
    Editor::SetStrength(*c, strength);
    Editor::SetCursedEnergy(*c, cursed_energy);
    Editor::SetDomain(*c, std::unique_ptr<int>()); // placeholder
    Editor::SetTechnique(*c, std::unique_ptr<int>()); // placeholder
    Editor::SetCursedEnergyEfficiency(*c, ce_efficiency);
    Editor::SetTraitPassiveHealing(*c, true);

    return c;
}
// sorcerer
std::unique_ptr<Sorcerer> Create::Gojo() {
    auto c = std::make_unique<Sorcerer>();

    constexpr const char* name = "Gojo";
    constexpr const char* color = "\x1b[91m";
    constexpr double health = 1000.0;
    constexpr double cursed_energy = 5000.0;
    constexpr double strength = 185.0;
    constexpr double durability = 300.0;
    constexpr auto ce_efficiency = CurseUser::CEfficiency::Extreme;

    Editor::SetName(*c, name);
    Editor::SetColor(*c, color);
    Editor::SetHealth(*c, health);
    Editor::SetDurability(*c, durability);
    Editor::SetStrength(*c, strength);
    Editor::SetCursedEnergy(*c, cursed_energy);
    Editor::SetDomain(*c, std::unique_ptr<int>()); // placeholder
    Editor::SetDomainNullifier(*c, std::unique_ptr<int>()); // placeholder
    Editor::SetTechnique(*c, std::unique_ptr<int>()); // placeholder
    Editor::SetCursedEnergyEfficiency(*c, ce_efficiency);
    Editor::SetTraitSixEyes(*c, true);

    return c;
}