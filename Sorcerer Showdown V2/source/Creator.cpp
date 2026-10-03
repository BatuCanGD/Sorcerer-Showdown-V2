#include "../header/Creator.hpp"
#include "../header/Utilities/Random.hpp"
#include "../header/CharacterType/Character.hpp"
#include "../header/CharacterType/CurseUser.hpp"
#include "../header/Stuff/CursedTool.hpp"
#include "../header/Sorcery/Technique.hpp"
#include "../header/Enums.hpp"

#include <memory>
#include <format>
// Base Characters
std::unique_ptr<Character> Create::TranfiguredHuman() {
    auto c = std::make_unique<Character>();

    const EntityInfo id = {"Transfigured Human", "\x1b[38;5;22m", "A human, but with its body extremely disfigured"};

    const double health      = get_random<double>(1.0, 100.0);
    const double strength    = get_random<double>(1.0, 100.0);
    const double durability  = get_random<double>(1.0, 100.0);

    const CharState stats {.health = health, .max_health = health, .durability = durability, .strength = strength};

    c->Identity() = id;
    c->State() = stats;

    return c;
}
// Curse Users
std::unique_ptr<CurseUser> Create::Mahito() {
    auto c = std::make_unique<CurseUser>();

    const EntityInfo id {"Mahito", "\x1b[38;5;129m", "Cursed Spirit with the ability to Reshape souls"};
    
    constexpr double health     = 550.0;
    constexpr double strength   = 115.0;
    constexpr double durability = 75.0;

    const BattleIQ style {.targeting_type = TargetingType::HighestHP,.fighting_style = FightingStyle::Aggressive,  .resource_usage = ResourceUsage::Mixed};
    const CharState stats {.health = health, .max_health = health, .durability = durability, .strength = strength};

    constexpr double cursed_energy = 4000.0;
    constexpr auto ce_efficiency = CursedEnergySystem::Efficiency::Stable;

    const Technique technique{Create::IdleTransfiguration()};
    const Domain domain{Create::SelfEmbodimentOfPerfection()};

    c->Identity() = id;
    c->Style() = style;
    c->State() = stats;
    c->CursedEnergy().max_cursed_energy = cursed_energy;
    c->CursedEnergy().cursed_energy = cursed_energy;
    c->CursedEnergy().efficiency = ce_efficiency;
    c->Jujutsu().domain = domain;
    c->Jujutsu().technique = technique;
    c->Traits().passive_healing = true;

    return c;
}
std::unique_ptr<CurseUser> Create::Gojo() {
    auto c = std::make_unique<CurseUser>();

    const EntityInfo id {"Gojo", "\x1b[38;5;39m", "Strongest sorcerer of today"};

    constexpr double health     = 1000.0;
    constexpr double strength   = 185.0;
    constexpr double durability = 300.0;

    const BattleIQ style {.targeting_type = TargetingType::HighestHP,.fighting_style = FightingStyle::Aggressive,  .resource_usage = ResourceUsage::AllOut};
    const CharState stats {.health = health, .max_health = health, .durability = durability, .strength = strength};

    constexpr double cursed_energy = 5000.0;

    constexpr auto ce_efficiency = CursedEnergySystem::Efficiency::Extreme;
    constexpr auto rct_level = ReverseCTSystem::RCTLevel::Absolute;

    const Technique technique{Create::Limitless()};
    const Domain domain{Create::UnlimitedVoid()};
    const Neutralizer neutralizer{Create::SimpleDomain()};

    c->Identity() = id;
    c->Style() = style;
    c->State() = stats;
    c->CursedEnergy().max_cursed_energy = cursed_energy;
    c->CursedEnergy().cursed_energy = cursed_energy;
    c->Jujutsu().domain = domain;
    c->Jujutsu().neutralizer = neutralizer;
    c->Jujutsu().technique = technique;
    c->CursedEnergy().efficiency = ce_efficiency;
    c->Traits().six_eyes = true;
    c->RCTSystem().can_use_rct = true;
    c->RCTSystem().rct_level = rct_level;

    return c;
}

// techniques

Technique Create::Limitless() {
    Technique c{};

    const EntityInfo id = {"Limitless", "\x1b[38;5;45m", "An Inherited Technique that grants the user control over space itself" };
    const EntityInfo blue_id = {"Blue", "\x1b[38;5;20m", "The power to attract"};
    const EntityInfo red_id = {"Red", "\x1b[38;5;9m", "The power to repel"};
    const EntityInfo purple_id = {"Purple", "\x1b[38;5;129m", "Blue and Red combined, destroys anything in its path"};
    
    c.Identity() = id;

    constexpr auto at_type = globalums::DamageType::Normal;
    constexpr double blue_damage    = 125.0, blue_cost      = 335.0, blue_output    = 55.0;
    constexpr double red_damage     = 175.0, red_cost       = 650.0, red_output     = 120.0;
    constexpr double purple_damage  = 300.0, purple_cost    = 1250.0,purple_output  = 200.0;

    const TechAbility blue    = {blue_id,      blue_damage,   blue_cost,   blue_output,    at_type}; 
    const TechAbility red     = {red_id,       red_damage,    red_cost,    red_output,     at_type}; 
    const TechAbility purple  = {purple_id,    purple_damage, purple_cost, purple_output,  at_type};

    c.Abilities().push_back(blue);
    c.Abilities().push_back(red);
    c.Abilities().push_back(purple);

    c.Barrier().can_use_barrier = true;
    c.Barrier().is_active = true;

    return c;
}

Technique Create::IdleTransfiguration() {
    Technique c{};

    const EntityInfo id = {"Idle Transfiguration", "\x1b[38;5;129m", "A Technique that grants the user the manipulation of the shape of souls"};
    const EntityInfo tfig_id {"Transfiguration", "\x1b[38;5;238m", "Attacks the users soul directly"};
    
    c.Identity() = id;

    constexpr auto at_type = globalums::DamageType::BypassRein;
    constexpr double tfig_damage = 100.0, tfig_cost = 225.0, tfig_output = 45.0;

    const TechAbility transfiguration = {tfig_id, tfig_damage, tfig_cost ,tfig_output,at_type};
    
    c.Abilities().push_back(transfiguration);
    
    return c;
}

// domains

Domain Create::UnlimitedVoid() {
    Domain c{};
    c.identity = {"Unlimited Void", "\x1b[34m", "A domain that floods the targets mind with endless information, paralyzing them"};
    c.damage_type = globalums::DamageType::BypassAll;
    c.surehit_type = SurehitType::Paralyzing;
    c.refinement = Refinement::Absolue;
    c.type = DomainType::Closed;
    c.cost = 2500.0;
    c.max_health = 500.0;
    c.health = c.max_health;
    c.damage = 225.0;
    c.durability = 7.5;
    c.range = 125;
    return c;
}

Domain Create::MalevolentShrine() {
    Domain c{};
    c.identity = {"Malevolent Shrine", "\x1b[31m", "A domain that relentlessly cuts anything within its range"};
    c.damage_type = globalums::DamageType::BypassTech;
    c.surehit_type = SurehitType::Basic; // allows survival even with neutralizers that reduce damage
    c.refinement = Refinement::Absolue;
    c.type = DomainType::Open;
    c.cost = 1250.0;
    c.max_health = 600.0;
    c.health = c.max_health;
    c.damage = 165.0;
    c.durability = 20.0;
    c.range = 200;
    return c;
}

Domain Create::SelfEmbodimentOfPerfection() {
    Domain c{};
    c.identity = {"Self Embodiment of Perfection", "\x1b[35m", "A domain that allows the manipulation of any entities soul inside the domain without requiring technique activation"};
    c.damage_type = globalums::DamageType::BypassAll;
    c.damage = 250.0;
    c.cost = 1000.0;
    c.refinement = Refinement::Overwhelming;
    c.surehit_type = SurehitType::Normal;
    c.max_health = 400.0;
    c.health = c.max_health;
    c.range = 85;
    return c;
}

// neutralizers

Neutralizer Create::SimpleDomain() {
    Neutralizer c{};
    c.identity = {"Simple Domain", "\x1b[33m", "A barrier technique that creates a small zone to neutralize cursed techniques"};
    c.neutralizer_type = NeutralizerType::FullyProtected;
    c.cost = 100.0;
    c.turn_type = NeutralizerTurnType::SelfSustained;
    c.max_health = 200.0;
    c.health = c.max_health;
    c.durability = 200.0;
    return c;
}
Neutralizer Create::FallingBlossomEmotion() {
    Neutralizer c{};
    c.identity = {"Falling Blossom Emotion", "\x1b[32m", "An anti-domain technique that automatically counters incoming attacks using cursed energy"};
    c.cost = 50.0;
    c.max_health = 50.0;
    c.health = c.max_health;
    c.durability = 50.0;
    c.neutralizer_type = NeutralizerType::ReducedDamage;
    c.turn_type = NeutralizerTurnType::UserSustained;
    return c;
}

// cursed tools

CursedTool Create::InvertedSpearOfHeaven() {
    CursedTool c{};

    const EntityInfo id = {"Inverted Spear Of Heaven", "", "A Cursed Tool that negates Techniques"};

    constexpr double damage = 100.0;

    c.identity = id;
    c.damage = damage;
    c.damage_type = globalums::DamageType::BypassTech;
    c.given_effect = Create::BleedEffect();

    return c;
}

// status effects

StatusEffect Create::BleedEffect() {
    StatusEffect c{};

    constexpr double severity_amount = 25.0;
    constexpr int effect_turn_amount = 3;

    const EntityInfo id = { "Bleeding...", "\x1b[38;5;9", std::format("Drains {:.1f}HP per turn", severity_amount)};

    c.id = id;
    c.effect_amount = severity_amount;
    c.effect_type = EffectType::Decrease;
    c.effect_for_type = EffectForType::Health;
    c.turn_amount = effect_turn_amount;

    return c;
}