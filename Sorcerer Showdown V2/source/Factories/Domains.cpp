#include "../../header/Creator.hpp"
#include "../../header/Sorcery/Domain.hpp"

Domain Create::Dmn::UnlimitedVoid() {
    Domain c{};
    c.identity = {"Unlimited Void", "\x1b[34m", "A domain that floods the targets mind with endless information, paralyzing them"};
    c.damage_type = DamageType::BypassAll;
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
    c.damage_type = DamageType::BypassTech;
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
    c.damage_type = DamageType::BypassAll;
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
    c.damage_type = DamageType::BypassTech;
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
    c.damage_type = DamageType::BypassTech;
    c.refinement = Refinement::Overwhelming;
    c.surehit_type = SurehitType::Normal;
    c.durability = 2.0;
    c.max_health = 450.0;
    c.health = c.max_health;
    c.range = 100;
    return c;
}