#include "../../header/CharacterType/Character.hpp"
#include "../../header/CombatHelper.hpp"

Character::~Character() = default;

double Character::Health(enumtype::Get type) const noexcept {
    switch (type) {
        case enumtype::Get::Current:  return state.health;
        case enumtype::Get::Max:      return state.max_health;
        case enumtype::Get::Previous: return state.previous_health;
    }
    return -1.0;
}

void Character::Health(enumtype::Set type, double amount) {
    switch (type) {
        case enumtype::Set::Current:  
            state.health = amount;
            break;
        case enumtype::Set::Max:
            state.max_health = amount;
            break;
        case enumtype::Set::Previous: 
            state.previous_health = amount;
            break;
    }
}

void Character::Damage(double amount, enumtype::DamageType type){
    if (state.is_invulnerable){
        return;
    }
    this->state.health -= CombatHelper::DealWithDamage(*this, type, amount);
}
AttackStruct Character::Attack(Character& cc){
    return CombatHelper::DealWithAttacking(*this, cc);
}