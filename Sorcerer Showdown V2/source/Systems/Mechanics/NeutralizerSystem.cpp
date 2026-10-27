#include "../../../header/Systems/Mechanics/NeutralizerSystem.hpp"
#include "../../../header/Systems/Mechanics/SorcerySystem.hpp"
#include "../../../header/Character.hpp"
#include "../../../header/Logger.hpp"
#include "../../../header/Sorcery/Neutralizer.hpp"

void NeutralizerSystem::HandleLifeTime(Character &c, Neutralizer &n) {
    if (!n.is_active) { 
        return;
    }else if (n.health <= 0.0){
        Reset(n);
        return;
    }
    const double cost = SorcerySystem::ApplySpendingMultiplier(n.cost, c);
    auto& cr = c.curseUser;
    if (cr->ce_system.cursed_energy < cost){
        Reset(n);
    }else{
        cr->ce_system.cursed_energy -= cost;
    }
}

void NeutralizerSystem::Damage(Neutralizer& c, const double damage) {
    c.health -= damage;
    if (c.health <= 0.0){
        NeutralizerSystem::Reset(c);
    }
}
void NeutralizerSystem::Reset(Neutralizer& c) {
    c.is_active = false;
    c.health = c.max_health;
}