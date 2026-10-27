#include "../../header/Creator.hpp"
#include "../../header/Utilities/Random.hpp"
#include "../../header/Character.hpp"

Character Create::Char::TranfiguredHuman() {
    Character c{};

    const EntityInfo id = {"Transfigured Human", "\x1b[38;5;22m", "A human, but with its body extremely disfigured"};

    const double health      = get_random<double>(1.0, 100.0);
    const double strength    = get_random<double>(1.0, 100.0);
    const double durability  = get_random<double>(1.0, 100.0);

    const Base::State stats {.health = health, .max_health = health, .durability = durability, .strength = strength};

    c.identity = id;
    c.state = stats;

    return c;
}
Character Create::Char::Toji() {
    Character c{};

    const EntityInfo id = {"Toji Fushiguro", "\x1b[38;5;25m", "A Physically Gifted human with no cursed energy"};

    constexpr double health      = 500.0;
    constexpr double strength    = 225.0;
    constexpr double durability  = 245.0;

    const Base::DecisionMaker brain {.targeting_type = TargetingType::HighestHP,.fighting_style = FightingStyle::Aggressive,  .resource_usage = ResourceUsage::AllOut};
    const Base::State stats {.health = health, .max_health = health, .durability = durability, .strength = strength};

    c.identity = id;
    c.state = stats;
    c.brain = brain;
    c.equipment.has_access_to_inventory = true;
    c.equipment.inventory.push_back(Create::Tool::InvertedSpearOfHeaven());
    c.equipment.inventory.push_back(Create::Tool::SplitSoulKatana());
    c.equipment.inventory.push_back(Create::Tool::PlayfulCloud());

    return c;
}
Character Create::Char::Maki() {
    Character c{};

    const EntityInfo id = {"Maki Zenin", "\x1b[38;5;64m", "A Heavenly Restricted human similar to Toji Fushiguro"}
;
    constexpr double health      = 750.0;
    constexpr double strength    = 265.0;
    constexpr double durability  = 155.0;

    const Base::DecisionMaker brain {.targeting_type = TargetingType::HighestHP,.fighting_style = FightingStyle::Mixed,  .resource_usage = ResourceUsage::AllOut};
    const Base::State stats {.health = health, .max_health = health, .durability = durability, .strength = strength};

    c.identity = id;
    c.state = stats;
    c.brain = brain;
    c.equipment.has_access_to_inventory = true;
    c.equipment.inventory.push_back(Create::Tool::SplitSoulKatana());

    return c;
}

Character Create::Char::Mahito() {
    Character c{};

    const EntityInfo id {"Mahito", "\x1b[38;5;129m", "A Disaster Curse with the ability to Reshape souls"};
    
    constexpr double health     = 550.0;
    constexpr double strength   = 115.0;
    constexpr double durability = 75.0;

    const Base::DecisionMaker brain {.targeting_type = TargetingType::LowestHP,.fighting_style = FightingStyle::Aggressive,  .resource_usage = ResourceUsage::Mixed};
    const Base::State stats {.health = health, .max_health = health, .durability = durability, .strength = strength};

    constexpr double cursed_energy = 4000.0;
    constexpr double regen_amount = 125.0;
    constexpr double output_regen = 22.5;
    constexpr double max_output = 70.0;
    constexpr auto ce_efficiency = Sorcery::CursedEnergySystem::Efficiency::Stable;

    const Technique technique{Create::Tech::IdleTransfiguration()};
    const Domain domain{Create::Dmn::SelfEmbodimentOfPerfection()};

    c.identity = id;
    c.brain = brain;
    c.state = stats;

    Sorcery::CurseUserSystem crs;

    crs.ce_system.max_cursed_energy = cursed_energy;
    crs.ce_system.cursed_energy = cursed_energy;
    crs.ce_system.regeneration_amount = regen_amount;
    crs.ce_system.efficiency = ce_efficiency;
    crs.output.max_output_potential = max_output;
    crs.output.output_cooldown_amount = output_regen;
    crs.jujutsu.domain = domain;
    crs.jujutsu.technique = technique;
    crs.traits.passive_healing = true;
    crs.amplification.can_use_amp = true;

    c.curseUser = crs;

    return c;
}
Character Create::Char::Jogo() {
    Character c{};

    const EntityInfo id {"Jogo", "\x1b[38;5;202m", "A Disaster Curse with the ability to create fire and lava"};

    constexpr double health     = 350.0;
    constexpr double strength   = 185.0;
    constexpr double durability = 100.0;

    const Base::DecisionMaker brain {.targeting_type = TargetingType::HighestHP,.fighting_style = FightingStyle::Defensive,  .resource_usage = ResourceUsage::AllOut};
    const Base::State stats {.health = health, .max_health = health, .durability = durability, .strength = strength};

    constexpr double cursed_energy = 4000.0;
    constexpr double regen_amount = 135.0;
    constexpr double output_regen = 30.0;
    constexpr double max_output = 170.0;
    constexpr auto ce_efficiency = Sorcery::CursedEnergySystem::Efficiency::Stable;


    const Technique technique{Create::Tech::DisasterFlames()};
    const Domain domain{Create::Dmn::CoffinOfTheIronMountain()};

    c.identity = id;
    c.brain = brain;
    c.state = stats;

    Sorcery::CurseUserSystem crs;

    crs.ce_system.max_cursed_energy = cursed_energy;
    crs.ce_system.cursed_energy = cursed_energy;
    crs.ce_system.regeneration_amount = regen_amount;
    crs.ce_system.efficiency = ce_efficiency;
    crs.output.max_output_potential = max_output;
    crs.output.output_cooldown_amount = output_regen;
    crs.jujutsu.domain = domain;
    crs.jujutsu.technique = technique;
    crs.traits.passive_healing = true;
    crs.amplification.can_use_amp = true;

    c.curseUser = crs;

    return c;
}
Character Create::Char::Hanami() {
    Character c{};

    const EntityInfo id {"Hanami", "\x1b[38;5;217m", "A Disaster Curse with the ability to create Trees from the ground and Pacifying flowers"};

    constexpr double health     = 450.0;
    constexpr double strength   = 75.0;
    constexpr double durability = 300.0;

    const Base::DecisionMaker brain {.targeting_type = TargetingType::Random,.fighting_style = FightingStyle::Mixed,  .resource_usage = ResourceUsage::Conservative};
    const Base::State stats {.health = health, .max_health = health, .durability = durability, .strength = strength};

    constexpr double cursed_energy = 3250.0;
    constexpr double regen_amount = 75.0;
    constexpr double output_regen = 15.0;
    constexpr double max_output = 150.0;
    constexpr auto ce_efficiency = Sorcery::CursedEnergySystem::Efficiency::Stable;


    const Technique technique{Create::Tech::DisasterPlants()};
    const Domain domain{Create::Dmn::ShiningSeaOfGrowingBranches()};

    c.identity = id;
    c.brain = brain;
    c.state = stats;

    Sorcery::CurseUserSystem crs;

    crs.ce_system.max_cursed_energy = cursed_energy;
    crs.ce_system.cursed_energy = cursed_energy;
    crs.ce_system.regeneration_amount = regen_amount;
    crs.ce_system.efficiency = ce_efficiency;
    crs.output.max_output_potential = max_output;
    crs.output.output_cooldown_amount = output_regen;
    crs.jujutsu.domain = domain;
    crs.jujutsu.technique = technique;
    crs.traits.passive_healing = true;
    crs.amplification.can_use_amp = true;

    c.curseUser = crs;

    return c;
}
Character Create::Char::Gojo() {
    Character c{};

    const EntityInfo id {"Satoru Gojo", "\x1b[38;5;39m", "Strongest sorcerer of today"};

    constexpr double health     = 1000.0;
    constexpr double strength   = 200.0;
    constexpr double durability = 300.0;

    const Base::DecisionMaker brain {.targeting_type = TargetingType::HighestHP,.fighting_style = FightingStyle::Aggressive,  .resource_usage = ResourceUsage::AllOut};
    const Base::State stats {.health = health, .max_health = health, .durability = durability, .strength = strength};

    constexpr double cursed_energy = 6000.0;
    constexpr double regen_amount = 175.0;
    constexpr double max_output = 175.0;
    constexpr double output_regen = 45.0;
    constexpr auto ce_efficiency = Sorcery::CursedEnergySystem::Efficiency::Peak;

    const Technique technique{Create::Tech::Limitless()};
    const Domain domain{Create::Dmn::UnlimitedVoid()};
    const Neutralizer neutralizer{Create::Nlr::SimpleDomain()};

    c.identity = id;
    c.brain = brain;
    c.state = stats;

    Sorcery::CurseUserSystem crs;

    crs.ce_system.max_cursed_energy = cursed_energy;
    crs.ce_system.cursed_energy = cursed_energy;
    crs.ce_system.regeneration_amount = regen_amount;
    crs.ce_system.efficiency = ce_efficiency;
    crs.output.max_output_potential = max_output;
    crs.output.output_cooldown_amount = output_regen;
    crs.jujutsu.domain = domain;
    crs.jujutsu.neutralizer = neutralizer;
    crs.jujutsu.technique = technique;
    crs.amplification.can_use_amp = true;
    crs.traits.six_eyes = true;
    crs.rct_system.can_use_rct = true;

    c.curseUser = crs;

    return c;
}
Character Create::Char::Sukuna() {
    Character c{};

    const EntityInfo id {"Ryomen Sukuna", "\x1b[38;5;9m", "Strongest sorcerer in history"};

    constexpr double health     = 1200.0;
    constexpr double strength   = 115.0;
    constexpr double durability = 185.0;

    const Base::DecisionMaker brain {.targeting_type = TargetingType::HighestHP,.fighting_style = FightingStyle::Aggressive,  .resource_usage = ResourceUsage::Mixed};
    const Base::State stats {.health = health, .max_health = health, .durability = durability, .strength = strength};

    constexpr double cursed_energy = 10000.0;
    constexpr double regen_amount = 200.0;
    constexpr double max_output = 80.0;
    constexpr double output_regen = 20.0;
    constexpr auto ce_efficiency = Sorcery::CursedEnergySystem::Efficiency::Extreme;

    const Technique technique{Create::Tech::Shrine()};
    const Domain domain{Create::Dmn::MalevolentShrine()};
    const Neutralizer neutralizer{Create::Nlr::HollowWickerBasket()};

    c.identity = id;
    c.brain = brain;
    c.state = stats;

    Sorcery::CurseUserSystem crs;

    crs.ce_system.max_cursed_energy = cursed_energy;
    crs.ce_system.cursed_energy = cursed_energy;
    crs.ce_system.regeneration_amount = regen_amount;
    crs.ce_system.efficiency = ce_efficiency;
    crs.output.max_output_potential = max_output;
    crs.output.output_cooldown_amount = output_regen;
    crs.jujutsu.domain = domain;
    crs.jujutsu.neutralizer = neutralizer;
    crs.jujutsu.technique = technique;
    crs.jujutsu.shikigami.push_back(Create::Shk::Mahoraga());
    crs.jujutsu.shikigami.push_back(Create::Shk::Agito());
    crs.amplification.can_use_amp = true;
    crs.rct_system.can_use_rct = true;

    c.curseUser = crs;

    return c;
}