#include "../header/Creator.hpp"
#include "../header/Editors/CharacterEditor.hpp"
#include "../header/Editors/TechniqueEditor.hpp"
#include "../header/Utilities/Random.hpp"
#include "../header/CharacterType/Character.hpp"
#include "../header/CharacterType/CurseUser.hpp"
#include "../header/CharacterType/Sorcerer.hpp"
#include "../header/SystemType/Technique.hpp"
#include "../header/Enums.hpp"

#include <memory>

// base characters
std::unique_ptr<Character> Create::TranfiguredHuman() {
    auto c = std::make_unique<Character>();

    constexpr const char* name = "Transfigured Human";
    constexpr const char* color = "\x1b[38;5;22m";
    const double health = get_random<double>(1.0, 100.0);
    const double strength = get_random<double>(1.0, 100.0);
    const double durability = get_random<double>(1.0, 100.0);

    CharacterEditor::SetName(*c, name);
    CharacterEditor::SetColor(*c, color);
    CharacterEditor::SetHealth(*c, health);
    CharacterEditor::SetDurability(*c, durability);
    CharacterEditor::SetStrength(*c, strength);

    return c;
}
// curse users
std::unique_ptr<CurseUser> Create::Mahito() {
    auto c = std::make_unique<CurseUser>();

    constexpr const char* name = "Mahito";
    constexpr const char* color = "\x1b[38;5;129m";
    constexpr double health = 550.0;
    constexpr double cursed_energy = 4000.0;
    constexpr double strength = 115.0;
    constexpr double durability = 75.0;
    constexpr auto ce_efficiency = CurseUser::CEfficiency::Stable;

    auto technique = Create::IdleTransfiguration();

    CharacterEditor::SetName(*c, name);
    CharacterEditor::SetColor(*c, color);
    CharacterEditor::SetHealth(*c, health);
    CharacterEditor::SetDurability(*c, durability);
    CharacterEditor::SetStrength(*c, strength);
    CharacterEditor::SetCursedEnergy(*c, cursed_energy);
    CharacterEditor::SetDomain(*c, std::unique_ptr<int>()); // placeholder
    CharacterEditor::SetTechnique(*c, std::move(technique)); // placeholder
    CharacterEditor::SetCursedEnergyEfficiency(*c, ce_efficiency);
    CharacterEditor::SetTraitPassiveHealing(*c, true);

    return c;
}
// sorcerers
std::unique_ptr<Sorcerer> Create::Gojo() {
    auto c = std::make_unique<Sorcerer>();

    constexpr const char* name = "Gojo";
    constexpr const char* color = "\x1b[38;5;117m";
    constexpr double health = 1000.0;
    constexpr double cursed_energy = 5000.0;
    constexpr double strength = 185.0;
    constexpr double durability = 300.0;
    constexpr auto ce_efficiency = CurseUser::CEfficiency::Extreme;
    constexpr auto rct_level = Sorcerer::RCTLevel::Absolute;

    auto technique = Create::Limitless();

    CharacterEditor::SetName(*c, name);
    CharacterEditor::SetColor(*c, color);
    CharacterEditor::SetHealth(*c, health);
    CharacterEditor::SetDurability(*c, durability);
    CharacterEditor::SetStrength(*c, strength);
    CharacterEditor::SetCursedEnergy(*c, cursed_energy);
    CharacterEditor::SetDomain(*c, std::unique_ptr<int>()); // placeholder
    CharacterEditor::SetDomainNullifier(*c, std::unique_ptr<int>()); // placeholder
    CharacterEditor::SetTechnique(*c, std::move(technique));
    CharacterEditor::SetCursedEnergyEfficiency(*c, ce_efficiency);
    CharacterEditor::SetTraitSixEyes(*c, true);
    CharacterEditor::SetReverseCursedTechnique(*c, true);
    CharacterEditor::SetReverseCursedTechniqueLevel(*c, rct_level);

    return c;
}

// techniques

std::unique_ptr<Technique> Create::Limitless() {
    auto c = std::make_unique<Technique>();

    constexpr const char* name  = "Limitless";
    constexpr const char* color = "\x1b[38;5;14m";
    constexpr const char* descr = "An Inherited Technique that grants the user control over space itself";

    TechniqueEditor::SetName(*c, name);
    TechniqueEditor::SetColor(*c, color);
    TechniqueEditor::SetDescription(*c, descr);


    constexpr auto at_type = globalums::DamageType::Normal;

    constexpr const char* blue_str = "Blue";
    constexpr const char* red_str = "Red";
    constexpr const char* purple_str = "Purple";

    constexpr const char* blue_clr = "\x1b[38;5;14m";
    constexpr const char* red_clr = "\x1b[48;5;9m";
    constexpr const char* purple_clr = "\x1b[38;5;129m";

    constexpr double blue_damage = 125.0;
    constexpr double red_damage = 175.0;
    constexpr double purple_damage = 300.0;

    constexpr double blue_output = 75.0;
    constexpr double red_output = 150.0;
    constexpr double purple_output = 300.0;

    TechAbility blue    = {blue_damage, blue_output, blue_str, blue_clr, at_type}; 
    TechAbility red     = {red_damage, red_output,red_str, red_clr, at_type}; 
    TechAbility purple  = {purple_damage, purple_output, purple_str, purple_clr, at_type};
    
    TechniqueEditor::AddAbility(*c, blue);
    TechniqueEditor::AddAbility(*c, red);
    TechniqueEditor::AddAbility(*c, purple);

    return c;
}

std::unique_ptr<Technique> Create::IdleTransfiguration() {
    auto c = std::make_unique<Technique>();

    constexpr const char* name = "Idle Transfiguration";
    constexpr const char* color = "";
    constexpr const char* descr = "A Technique that grants the user the manipulation of the shape of souls";

    TechniqueEditor::SetName(*c, name);
    TechniqueEditor::SetColor(*c, color);
    TechniqueEditor::SetDescription(*c, descr);

    constexpr auto at_type = globalums::DamageType::BypassRein;

    constexpr const char* tfig_str = "Transfiguration";
    constexpr const char* tfig_clr = "\x1b[38;5;238m";
    constexpr double tfig_damage = 100.0;
    constexpr double tfig_output = 45.0;

    TechAbility transfiguration = {tfig_damage, tfig_output,tfig_str, tfig_clr, at_type};

    TechniqueEditor::AddAbility(*c, transfiguration);
    
    return c;
}