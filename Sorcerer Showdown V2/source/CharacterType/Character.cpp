#include "../../header/CharacterType/Character.hpp"
#include "../../header/CombatHelper.hpp"

Character::~Character() = default;

double Character::Health(type::Get type) const noexcept {
    switch (type) {
        case type::Get::Current:  return state.health;
        case type::Get::Max:      return state.max_health;
        case type::Get::Previous: return state.previous_health;
    }
    return -1.0;
}

void Character::Health(type::Set type, double amount) {
    switch (type) {
        case type::Set::Current:  
            state.health = amount;
            break;
        case type::Set::Max:
            state.max_health = amount;
            break;
        case type::Set::Previous: 
            state.previous_health = amount;
            break;
    }
}

void Character::Damage(double amount, globalums::DamageType type){
    if (state.is_invulnerable){
        return;
    }
    this->state.health -= CombatHelper::DealWithDamage(*this, type, amount);
}
AttackStruct Character::Attack(Character& cc){
    return CombatHelper::DealWithAttacking(*this, cc);
}