#include "../../header/CharacterType/Character.hpp"
#include "../../header/CharacterUtils/CharacterHelper.hpp"

Character::~Character() = default;


double Character::Health(Get type) const noexcept {
    switch (type) {
        case Get::Current:  return state.health;
        case Get::Max:      return state.max_health;
        case Get::Previous: return state.previous_health;
    }
    return -1.0;
}

void Character::Health(Set type, double amount) {
    switch (type) {
        case Set::Current:  state.health            = amount;
        case Set::Max:      state.max_health        = amount;
        case Set::Previous: state.previous_health   = amount;
    }
}

void Character::Damage(double amount, enumtype::DamageType type){
    if (state.is_invulnerable){
        return;
    }
    this->state.health -= Helper::DealWithDamage(*this, type, amount);
}