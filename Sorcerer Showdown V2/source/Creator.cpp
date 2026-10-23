#include "../header/Creator.hpp"
#include "../header/Utilities/Random.hpp"
#include "../header/CharacterType/Character.hpp"
#include "../header/CharacterType/CurseUser.hpp"
#include "../header/General/CursedTool.hpp"
#include "../header/Sorcery/Technique.hpp"
#include "../header/Enums.hpp"
#include <format>

// Base Characters
Character Create::Char::TranfiguredHuman() {
    Character c{};

    const EntityInfo id = {"Transfigured Human", "\x1b[38;5;22m", "A human, but with its body extremely disfigured"};

    const double health      = get_random<double>(1.0, 100.0);
    const double strength    = get_random<double>(1.0, 100.0);
    const double durability  = get_random<double>(1.0, 100.0);

    const CharState stats {.health = health, .max_health = health, .durability = durability, .strength = strength};

    c.Identity() = id;
    c.State() = stats;

    return c;
}
Character Create::Char::Toji() {
    Character c{};

    const EntityInfo id = {"Toji Fushiguro", "\x1b[38;5;25m", "A Physically Gifted human with no cursed energy"};

    constexpr double health      = 500.0;
    constexpr double strength    = 225.0;
    constexpr double durability  = 245.0;

    const BattleIQ style {.targeting_type = TargetingType::HighestHP,.fighting_style = FightingStyle::Aggressive,  .resource_usage = ResourceUsage::AllOut};
    const CharState stats {.health = health, .max_health = health, .durability = durability, .strength = strength};

    c.Identity() = id;
    c.State() = stats;
    c.Style() = style;
    c.Equipment().has_access_to_inventory = true;
    c.Equipment().inventory.push_back(Create::Tool::InvertedSpearOfHeaven());
    c.Equipment().inventory.push_back(Create::Tool::SplitSoulKatana());
    c.Equipment().inventory.push_back(Create::Tool::PlayfulCloud());

    return c;
}
Character Create::Char::Maki() {
    Character c{};

    const EntityInfo id = {"Maki Zenin", "\x1b[38;5;64m", "A Heavenly Restricted human similar to Toji Fushiguro"}
;
    constexpr double health      = 750.0;
    constexpr double strength    = 265.0;
    constexpr double durability  = 155.0;

    const BattleIQ style {.targeting_type = TargetingType::HighestHP,.fighting_style = FightingStyle::Mixed,  .resource_usage = ResourceUsage::AllOut};
    const CharState stats {.health = health, .max_health = health, .durability = durability, .strength = strength};

    c.Identity() = id;
    c.State() = stats;
    c.Style() = style;
    c.Equipment().has_access_to_inventory = true;
    c.Equipment().inventory.push_back(Create::Tool::SplitSoulKatana());

    return c;
}
// Curse Users
CurseUser Create::Char::Mahito() {
    CurseUser c{};

    const EntityInfo id {"Mahito", "\x1b[38;5;129m", "A Disaster Curse with the ability to Reshape souls"};
    
    constexpr double health     = 550.0;
    constexpr double strength   = 115.0;
    constexpr double durability = 75.0;

    const BattleIQ style {.targeting_type = TargetingType::HighestHP,.fighting_style = FightingStyle::Aggressive,  .resource_usage = ResourceUsage::Mixed};
    const CharState stats {.health = health, .max_health = health, .durability = durability, .strength = strength};

    constexpr double cursed_energy = 4000.0;
    constexpr double regen_amount = 125.0;
    constexpr double output_regen = 22.5;
    constexpr double max_output = 70.0;
    constexpr auto ce_efficiency = CursedEnergySystem::Efficiency::Stable;

    const Technique technique{Create::Tech::IdleTransfiguration()};
    const Domain domain{Create::Dmn::SelfEmbodimentOfPerfection()};

    c.Identity() = id;
    c.Style() = style;
    c.State() = stats;
    c.CursedEnergy().max_cursed_energy = cursed_energy;
    c.CursedEnergy().cursed_energy = cursed_energy;
    c.CursedEnergy().regeneration_amount = regen_amount;
    c.CursedEnergy().efficiency = ce_efficiency;
    c.Output().max_output_potential = max_output;
    c.Output().output_cooldown_amount = output_regen;
    c.Jujutsu().domain = domain;
    c.Jujutsu().technique = technique;
    c.Traits().passive_healing = true;

    return c;
}

CurseUser Create::Char::Jogo() {
    CurseUser c{};

    const EntityInfo id {"Jogo", "\x1b[38;5;202m", "A Disaster Curse with the ability to create fire and lava"};

    const Technique technique{Create::Tech::DisasterFlames()};
    const Domain domain{Create::Dmn::CoffinOfTheIronMountain()};

    c.Identity() = id;
    c.Jujutsu().domain = domain;
    c.Jujutsu().technique = technique;
    c.Traits().passive_healing = true;
    c.Amplification().can_use_amp = true;

    return c;
}
CurseUser Create::Char::Hanami() {
    CurseUser c{};

    const EntityInfo id {"Hanami", "\x1b[38;5;217m", "A Disaster Curse with the ability to create Trees from the ground and Pacifying flowers"};

    const Technique technique{Create::Tech::DisasterPlants()};
    const Domain domain{Create::Dmn::ShiningSeaOfGrowingBranches()};

    c.Identity() = id;
    c.Jujutsu().domain = domain;
    c.Jujutsu().technique = technique;
    c.Traits().passive_healing = true;
    c.Amplification().can_use_amp = true;

    return c;
}

CurseUser Create::Char::Gojo() {
    CurseUser c{};

    const EntityInfo id {"Satoru Gojo", "\x1b[38;5;39m", "Strongest sorcerer of today"};

    constexpr double health     = 1000.0;
    constexpr double strength   = 200.0;
    constexpr double durability = 300.0;

    const BattleIQ style {.targeting_type = TargetingType::HighestHP,.fighting_style = FightingStyle::Aggressive,  .resource_usage = ResourceUsage::AllOut};
    const CharState stats {.health = health, .max_health = health, .durability = durability, .strength = strength};

    constexpr double cursed_energy = 6000.0;
    constexpr double regen_amount = 175.0;
    constexpr double max_output = 175.0;
    constexpr double output_regen = 45.0;
    constexpr auto ce_efficiency = CursedEnergySystem::Efficiency::Extreme;

    const Technique technique{Create::Tech::Limitless()};
    const Domain domain{Create::Dmn::UnlimitedVoid()};
    const Neutralizer neutralizer{Create::Nlr::SimpleDomain()};

    c.Identity() = id;
    c.Style() = style;
    c.State() = stats;
    c.CursedEnergy().max_cursed_energy = cursed_energy;
    c.CursedEnergy().cursed_energy = cursed_energy;
    c.CursedEnergy().regeneration_amount = regen_amount;
    c.CursedEnergy().efficiency = ce_efficiency;
    c.Output().max_output_potential = max_output;
    c.Output().output_cooldown_amount = output_regen;
    c.Jujutsu().domain = domain;
    c.Jujutsu().neutralizer = neutralizer;
    c.Jujutsu().technique = technique;
    c.Amplification().can_use_amp = true;
    c.Traits().six_eyes = true;
    c.RCT().can_use_rct = true;

    return c;
}
CurseUser Create::Char::Sukuna() {
    CurseUser c{};

    const EntityInfo id {"Ryomen Sukuna", "\x1b[38;5;9m", "Strongest sorcerer in history"};

    constexpr double health     = 1200.0;
    constexpr double strength   = 115.0;
    constexpr double durability = 185.0;

    const BattleIQ style {.targeting_type = TargetingType::HighestHP,.fighting_style = FightingStyle::Aggressive,  .resource_usage = ResourceUsage::Mixed};
    const CharState stats {.health = health, .max_health = health, .durability = durability, .strength = strength};

    constexpr double cursed_energy = 10000.0;
    constexpr double regen_amount = 200.0;
    constexpr double max_output = 80.0;
    constexpr double output_regen = 20.0;
    constexpr auto ce_efficiency = CursedEnergySystem::Efficiency::Absolute;

    const Technique technique{Create::Tech::Shrine()};
    const Domain domain{Create::Dmn::MalevolentShrine()};
    const Neutralizer neutralizer{Create::Nlr::HollowWickerBasket()};

    c.Identity() = id;
    c.Style() = style;
    c.State() = stats;
    c.CursedEnergy().max_cursed_energy = cursed_energy;
    c.CursedEnergy().cursed_energy = cursed_energy;
    c.CursedEnergy().regeneration_amount = regen_amount;
    c.CursedEnergy().efficiency = ce_efficiency;
    c.Output().max_output_potential = max_output;
    c.Output().output_cooldown_amount = output_regen;
    c.Jujutsu().domain = domain;
    c.Jujutsu().neutralizer = neutralizer;
    c.Jujutsu().technique = technique;
    c.Jujutsu().shikigami.push_back(Create::Shk::Mahoraga());
    c.Jujutsu().shikigami.push_back(Create::Shk::Agito());
    c.Amplification().can_use_amp = true;
    c.RCT().can_use_rct = true;

    return c;
}

// techniques

Technique Create::Tech::Limitless() {
    Technique c{};

    const EntityInfo id = {"Limitless", "\x1b[38;5;45m", "An Inherited Technique that grants the user control over space itself" };
    const EntityInfo blue_id = {"Blue", "\x1b[38;5;20m", "The power to attract"};
    const EntityInfo red_id = {"Red", "\x1b[38;5;9m", "The power to repel"};
    const EntityInfo purple_id = {"Purple", "\x1b[38;5;129m", "Blue and Red combined, destroys anything in its path"};
    
    c.identity = id;

    constexpr auto at_type = globalums::DamageType::Normal;
    constexpr double blue_damage    = 125.0, blue_cost      = 335.0, blue_output    = 55.0;
    constexpr double red_damage     = 175.0, red_cost       = 650.0, red_output     = 120.0;
    constexpr double purple_damage  = 300.0, purple_cost    = 1250.0,purple_output  = 200.0;

    const TechAbility blue    = {blue_id,      blue_damage,   blue_cost,   blue_output,    0, at_type}; 
    const TechAbility red     = {red_id,       red_damage,    red_cost,    red_output,     0, at_type}; 
    const TechAbility purple  = {purple_id,    purple_damage, purple_cost, purple_output,  0, at_type};

    c.abilities.push_back(blue);
    c.abilities.push_back(red);
    c.abilities.push_back(purple);

    c.barrier.can_use_barrier = true;
    c.barrier.is_active = true;

    return c;
}
Technique Create::Tech::Shrine() {
    Technique c{};

    const EntityInfo id = {"Shrine", "\x1b[38;5;9m", "A Cutting Technique that can send out both normal slashes and slashes that adapts to the targets durability" };

    c.identity = id;

    const EntityInfo dismantle_id = {"Dismantle", "\x1b[38;5;209m", "A powerful slicing ability" };
    const EntityInfo cleave_id = {"Cleave", "\x1b[38;5;214m", "An ability that requires touch, that cuts through anyones durability, with enough output" };
    const EntityInfo spiderweb_id = {"Spiderweb", "\x1b[38;5;9m", "Cleave used in a way that disorients other opponents"};

    constexpr double dismantle_damage = 75.0, dismantle_cost = 35.0, dismantle_output = 17.5;
    constexpr double cleave_damage = 250.0, cleave_cost = 1000.0, cleave_output = 175.0;
    constexpr double spiderweb_damage = 25.0, spiderweb_cost = 500.0, spiderweb_output = 100.0;

    const TechAbility dismantle = {dismantle_id, dismantle_damage, dismantle_cost, dismantle_output,0,  globalums::DamageType::Normal};
    const TechAbility cleave = {cleave_id, cleave_damage, cleave_cost, cleave_output, 0, globalums::DamageType::BypassRein};
    const TechAbility spiderweb = {spiderweb_id, spiderweb_damage, spiderweb_cost, spiderweb_output,0,  globalums::DamageType::Normal};

    c.abilities.push_back(dismantle);
    c.abilities.push_back(cleave);
    c.abilities.push_back(spiderweb);

    return c;
}

Technique Create::Tech::IdleTransfiguration() {
    Technique c{};

    const EntityInfo id = {"Idle Transfiguration", "\x1b[38;5;129m", "A Technique that grants the user the manipulation of the shape of souls"};
    const EntityInfo tfig_id {"Transfiguration", "\x1b[38;5;238m", "Attacks the users soul directly"};
    
    c.identity = id;

    constexpr auto at_type = globalums::DamageType::BypassRein;
    constexpr double tfig_damage = 100.0, tfig_cost = 225.0, tfig_output = 45.0;

    const TechAbility transfiguration = {tfig_id, tfig_damage, tfig_cost ,tfig_output, 0, at_type};
    
    c.abilities.push_back(transfiguration);
    
    return c;
}

Technique Create::Tech::DisasterFlames() {
    Technique c{};

    const EntityInfo id = {"Disaster Flames", "\x1b[38;5;202m", ""};
  
    c.identity = id;
    
    return c;
}

Technique Create::Tech::DisasterPlants() {
    Technique c{};

    const EntityInfo id = {"Disaster Plants", "\x1b[38;5;217m", ""};

    c.identity = id;
    
    return c;
}

// domains

Domain Create::Dmn::UnlimitedVoid() {
    Domain c{};
    c.identity = {"Unlimited Void", "\x1b[34m", "A domain that floods the targets mind with endless information, paralyzing them"};
    c.damage_type = globalums::DamageType::BypassAll;
    c.surehit_type = SurehitType::Paralyzing;
    c.refinement = Refinement::Absolue;
    c.type = DomainType::Closed;
    c.cost = 2500.0;
    c.max_health = 500.0;
    c.health = c.max_health;
    c.damage = 300.0;
    c.durability = 3.5;
    c.range = 125;
    return c;
}

Domain Create::Dmn::MalevolentShrine() {
    Domain c{};
    c.identity = {"Malevolent Shrine", "\x1b[31m", "A domain that relentlessly cuts anything within its range"};
    c.damage_type = globalums::DamageType::BypassTech;
    c.surehit_type = SurehitType::Basic; // allows survival even with neutralizers that reduce damage
    c.refinement = Refinement::Absolue;
    c.type = DomainType::Open;
    c.cost = 1250.0;
    c.max_health = 600.0;
    c.health = c.max_health;
    c.damage = 300.0;
    c.durability = 5.5;
    c.range = 200;
    return c;
}

Domain Create::Dmn::SelfEmbodimentOfPerfection() {
    Domain c{};
    c.identity = {"Self Embodiment of Perfection", "\x1b[35m", "A domain that allows the manipulation of any entities soul inside the domain without requiring technique activation"};
    c.damage_type = globalums::DamageType::BypassAll;
    c.damage = 250.0;
    c.cost = 1000.0;
    c.refinement = Refinement::Overwhelming;
    c.surehit_type = SurehitType::Normal;
    c.max_health = 400.0;
    c.durability = 2.25;
    c.health = c.max_health;
    c.range = 100;
    return c;
}

Domain Create::Dmn::ShiningSeaOfGrowingBranches() {
    Domain c{};
    c.identity = {"Coffin of The Iron Mountain", "\x1b[38;5;217m", ""};
    c.damage = 155.0;
    c.cost = 1000.0;
    c.damage_type = globalums::DamageType::BypassTech;
    c.refinement = Refinement::Overwhelming;
    c.surehit_type = SurehitType::Paralyzing;
    c.durability = 1.7;
    c.max_health = 350.0;
    c.health = c.max_health;
    c.range = 100;
    return c;
}
Domain Create::Dmn::CoffinOfTheIronMountain() {
    Domain c{};
    c.identity = {"Coffin of The Iron Mountain", "\x1b[38;5;202m", ""};
    c.damage = 265.0;
    c.cost = 1000.0;
    c.damage_type = globalums::DamageType::BypassTech;
    c.refinement = Refinement::Overwhelming;
    c.surehit_type = SurehitType::Normal;
    c.durability = 2.0;
    c.max_health = 450.0;
    c.health = c.max_health;
    c.range = 100;
    return c;
}

// neutralizers

Neutralizer Create::Nlr::SimpleDomain() {
    Neutralizer c{};
    c.identity = {"Simple Domain", "\x1b[33m", "A barrier technique that creates a small zone to neutralize cursed techniques"};
    c.neutralizer_type = NeutralizerType::FullyProtected;
    c.cost = 100.0;
    c.max_health = 200.0;
    c.health = c.max_health;
    c.durability = 200.0;
    return c;
}
Neutralizer Create::Nlr::HollowWickerBasket() {
    Neutralizer c{};
    c.identity = {"Hollow Wicker Basket", "\x1b[33m", "A barrier technique older than simple domain that requires continuous hand signs"};
    c.neutralizer_type = NeutralizerType::FullyProtected;
    c.cost = 300.0;
    c.max_health = 300.0;
    c.health = c.max_health;
    c.durability = 75.0;
    return c;
}
Neutralizer Create::Nlr::FallingBlossomEmotion() {
    Neutralizer c{};
    c.identity = {"Falling Blossom Emotion", "\x1b[32m", "An anti-domain technique that automatically counters incoming attacks using cursed energy"};
    c.cost = 50.0;
    c.max_health = 50.0;
    c.health = c.max_health;
    c.durability = 50.0;
    c.neutralizer_type = NeutralizerType::ReducedDamage;
    return c;
}

// cursed tools

CursedTool Create::Tool::InvertedSpearOfHeaven() {
    CursedTool c{};
    const EntityInfo id = {"Inverted Spear Of Heaven", "\x1b[38;5;179m", "A Cursed Tool that negates Techniques"};
    constexpr double damage = 75.0;
    const auto at_type = globalums::DamageType::BypassTech;
    c.identity = id;
    c.damage = damage;
    c.damage_type = at_type;
    c.given_effect = Create::Effect::BleedEffect();
    return c;
}
CursedTool Create::Tool::SplitSoulKatana() {
    CursedTool c{};
    const EntityInfo id = {"Split Soul Katana", "\x1b[38;5;4m", "A Cursed Tool that negates Durability and cuts the soul"};
    constexpr double damage = 80.0;
    const auto at_type = globalums::DamageType::BypassRein;
    c.identity = id;
    c.damage = damage;
    c.damage_type = at_type;
    c.given_effect = Create::Effect::BleedEffect();
    return c;
}
CursedTool Create::Tool::PlayfulCloud() {
    CursedTool c{};
    const EntityInfo id = {"Playful Cloud", "\x1b[38;5;9m", "A Cursed Tool that's strength increases based on the bearers strength"};
    constexpr double damage = 70.0;
    const auto at_type = globalums::DamageType::Normal;
    c.identity = id;
    c.damage = damage;
    c.damage_type = at_type;
    c.given_effect = Create::Effect::BleedEffect();
    return c;
}

// status effects

StatusEffect Create::Effect::BleedEffect() {
    StatusEffect c{};

    constexpr double severity_amount = 25.0;
    constexpr int effect_turn_amount = 3;

    const EntityInfo id = { "Bleeding...", "\x1b[38;5;9m", std::format("Drains {:.1f}HP per turn", severity_amount)};

    c.id = id;
    c.effect_amount = severity_amount;
    c.effect_type = EffectType::Decrease;
    c.effect_for_type = EffectForType::Health;
    c.turn_amount = effect_turn_amount;

    return c;
}

// shikigami

Shikigami Create::Shk::Mahoraga() {
    Shikigami c{};
    const EntityInfo id = { "Mahoraga", "\x1b[38;5;220m", "Shikigami that can adapt to anything and everything"};

    c.id = id;
    c.cost = 300.0;
    c.stats.max_health = 800.0;
    c.stats.health = c.stats.max_health;
    c.stats.durability = 150.0;
    c.stats.regen_speed = 50.0;
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