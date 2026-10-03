#include "../../../header/Systems/System/NeutralizerSystem.hpp"
#include "../../../header/Sorcery/Neutralizer.hpp"

void NeutralizerSystem::HandleDamage(Neutralizer& c, const double damage) {
    if (!c.is_active) return;
    c.health -= damage;
}
void NeutralizerSystem::ResetNeutralizer(Neutralizer& c) {
    c.is_active = false;
    c.health = c.max_health;
}