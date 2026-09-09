#include "../header/Creator.hpp"
#include "../header/Editors/CharacterEditor.hpp"
#include "../header/Editors/TechniqueEditor.hpp"
#include "../header/Utilities/Random.hpp"
#include "../header/CharacterType/Character.hpp"
#include "../header/CharacterType/CurseUser.hpp"
#include "../header/CharacterType/Sorcerer.hpp"
#include "../header/Sorcery/Technique.hpp"
#include "../header/Enums.hpp"

#include <memory>

// base characters
std::unique_ptr<Character> Create::TranfiguredHuman() {
    auto c = std::make_unique<Character>();

    const EntityId id = {"Transfigured Human", "\x1b[38;5;22m"};

    const double health = get_random<double>(1.0, 100.0);
    const double strength = get_random<double>(1.0, 100.0);
    const double durability = get_random<double>(1.0, 100.0);

    const CharState stats {health, health, durability, strength};

    CharacterEditor::SetIdentity(*c, id);
    CharacterEditor::SetStats(*c, stats);

    return c;
}
// curse users
std::unique_ptr<CurseUser> Create::Mahito() {
    auto c = std::make_unique<CurseUser>();

    const EntityId id {"Mahito", "\x1b[38;5;129m"};
    
    constexpr double health = 550.0;
    constexpr double strength = 115.0;
    constexpr double durability = 75.0;

    const CharState stats {health, health, durability, strength};

    constexpr double cursed_energy = 4000.0;
    constexpr auto ce_efficiency = CurseUserSystem::Efficiency::Stable;

    auto technique = Create::IdleTransfiguration();

    CharacterEditor::SetIdentity(*c, id);
    CharacterEditor::SetStats(*c, stats);
    CharacterEditor::SetCursedEnergy(*c, cursed_energy);
    CharacterEditor::SetCursedEnergyEfficiency(*c, ce_efficiency);
    CharacterEditor::SetDomain(*c, std::unique_ptr<int>()); // placeholder
    CharacterEditor::SetTechnique(*c, std::move(technique)); // placeholder
    CharacterEditor::SetTraitPassiveHealing(*c, true);

    return c;
}
// sorcerers
std::unique_ptr<Sorcerer> Create::Gojo() {
    auto c = std::make_unique<Sorcerer>();

    const EntityId id {"Gojo", "\x1b[38;5;117m"};

    constexpr double health = 1000.0;
    constexpr double strength = 185.0;
    constexpr double durability = 300.0;

    const CharState stats = {health, health, durability, strength};

    constexpr double cursed_energy = 5000.0;

    constexpr auto ce_efficiency = CurseUserSystem::Efficiency::Extreme;
    constexpr auto rct_level = SorcererSystem::RCTLevel::Absolute;

    auto technique = Create::Limitless();

    CharacterEditor::SetIdentity(*c, id);
    CharacterEditor::SetStats(*c, stats);
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

    EntityInfo id = {"Limitless", "\x1b[38;5;14m", "An Inherited Technique that grants the user control over space itself" };
    EntityInfo blue_id = {"Blue", "\x1b[38;5;14m", "The power to attract"};
    EntityInfo red_id = {"Red", "\x1b[48;5;9m", "The power to repel"};
    EntityInfo purple_id = {"Purple", "\x1b[38;5;129m", "Blue and Red combined, destroys anything in its path"};
    
    TechniqueEditor::SetIdentity(*c, id);

    constexpr auto at_type = globalums::DamageType::Normal;
    constexpr double blue_damage = 125.0, blue_cost = 335.0, blue_output = 55.0;
    constexpr double red_damage = 175.0, red_cost = 650.0, red_output = 120.0;
    constexpr double purple_damage = 300.0, purple_cost = 1250.0, purple_output = 200.0;

    TechAbility blue    =  {blue_damage,     blue_cost,  blue_output,    blue_id,    at_type}; 
    TechAbility red     =  {red_damage,      red_cost,   red_output,     red_id,     at_type}; 
    TechAbility purple  = {purple_damage,   purple_cost,purple_output,  purple_id,  at_type};
    
    TechniqueEditor::AddAbility(*c, blue);
    TechniqueEditor::AddAbility(*c, red);
    TechniqueEditor::AddAbility(*c, purple);

    return c;
}

std::unique_ptr<Technique> Create::IdleTransfiguration() {
    auto c = std::make_unique<Technique>();

    EntityInfo id = {"Idle Transfiguration", "\x1b[38;5;129m", "A Technique that grants the user the manipulation of the shape of souls"};
    EntityInfo tfig_id {"Transfiguration", "\x1b[38;5;238m", "Attacks the users soul directly"};
    
    TechniqueEditor::SetIdentity(*c, id);

    constexpr auto at_type = globalums::DamageType::BypassRein;
    constexpr double tfig_damage = 100.0, tfig_cost = 225.0, tfig_output = 45.0;

    TechAbility transfiguration = {tfig_damage, tfig_cost ,tfig_output,tfig_id, at_type};
    
    TechniqueEditor::AddAbility(*c, transfiguration);
    
    return c;
}