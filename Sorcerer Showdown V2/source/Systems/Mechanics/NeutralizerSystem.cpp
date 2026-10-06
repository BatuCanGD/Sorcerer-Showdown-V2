#include "../../../header/Systems/Mechanics/NeutralizerSystem.hpp"
#include "../../../header/Sorcery/Neutralizer.hpp"

void NeutralizerSystem::HandleDamage(Neutralizer& c, const double damage) {
    if (!c.is_active) return;
    c.health -= damage;
    if (c.health <= 0.0){
        NeutralizerSystem::ResetNeutralizer(c);
    }
}
void NeutralizerSystem::ResetNeutralizer(Neutralizer& c) {
    c.is_active = false;
    c.health = c.max_health;
}