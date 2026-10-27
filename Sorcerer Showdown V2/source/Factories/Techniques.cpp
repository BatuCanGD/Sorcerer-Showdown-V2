#include "../../header/Creator.hpp"
#include "../../header/Sorcery/Technique.hpp"


Technique Create::Tech::Limitless() {
    Technique c{};

    const EntityInfo id = {"Limitless", "\x1b[38;5;45m", "An Inherited Technique that grants the user control over space itself" };
    const EntityInfo blue_id = {"Blue", "\x1b[38;5;20m", "The power to attract"};
    const EntityInfo red_id = {"Red", "\x1b[38;5;9m", "The power to repel"};
    const EntityInfo purple_id = {"Purple", "\x1b[38;5;129m", "Blue and Red combined, destroys anything in its path"};
    
    c.identity = id;

    constexpr double blue_damage    = 125.0, blue_cost      = 335.0, blue_output    = 55.0;
    constexpr double red_damage     = 175.0, red_cost       = 650.0, red_output     = 120.0;
    constexpr double purple_damage  = 300.0, purple_cost    = 1250.0,purple_output  = 200.0;

    const TechAbility blue    = {blue_id, {}, blue_damage, blue_cost, blue_output}; 
    const TechAbility red     = {red_id, {}, red_damage, red_cost, red_output}; 
    const TechAbility purple  = {purple_id, {}, purple_damage, purple_cost, purple_output};

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

    const TechAbility dismantle = {dismantle_id, {},dismantle_damage, dismantle_cost, dismantle_output};
    const TechAbility cleave = {cleave_id, {},cleave_damage, cleave_cost, cleave_output, {}, DamageType::BypassRein};
    const TechAbility spiderweb = {spiderweb_id, {},spiderweb_damage, spiderweb_cost, spiderweb_output};

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

    constexpr double tfig_damage = 100.0, tfig_cost = 225.0, tfig_output = 45.0;

    const TechAbility transfiguration = {tfig_id, {},tfig_damage, tfig_cost ,tfig_output};
    
    c.abilities.push_back(transfiguration);
    
    return c;
}

Technique Create::Tech::DisasterFlames() {
    Technique c{};

    const EntityInfo id = {"Disaster Flames", "\x1b[38;5;202m", ""};

    const EntityInfo mf_id{"Melting Flames","\x1b[38;5;130m",""};
    const EntityInfo vl_id{"Volcanic Lava","\x1b[38;5;172m",""};

    constexpr double mf_d = 90.0, mf_c = 250.0, mf_o = 30.0;
    constexpr double vl_d = 90.0, vl_c = 550.0, vl_o = 70.0;

    const TechAbility mf = {mf_id, {}, mf_d, mf_c, mf_o};
    const TechAbility vl = {vl_id, {},vl_d, vl_c, vl_o};
  
    c.identity = id;

    c.abilities.push_back(mf);
    c.abilities.push_back(vl);
    
    return c;
}

Technique Create::Tech::DisasterPlants() {
    Technique c{};

    const EntityInfo id = {"Disaster Plants", "\x1b[38;5;217m", ""};

    const EntityInfo sr_id = {"Sharp Roots", "\x1b[38;5;52m", ""};
    const EntityInfo cb_id = {"Cursed Buds", "\x1b[38;5;58m", ""};

    constexpr double sr_d = 105.0, sr_c = 255.0, sr_o = 60.0;
    constexpr double cb_d = 45.0, cb_c = 175.0, cb_o = 40.0;
    
    const TechAbility sr = {sr_id, Create::Effect::Bleed(), sr_d, sr_c, sr_o};
    const TechAbility cb = {cb_id, Create::Effect::Bleed(), cb_d, cb_c, cb_o};

    c.identity = id;

    c.abilities.push_back(sr);
    c.abilities.push_back(cb);
    
    return c;
}