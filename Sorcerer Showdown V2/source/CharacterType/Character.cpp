#include "../../header/CharacterType/Character.hpp"
#include "../../header/CombatHelper.hpp"

#include <format>

Character::~Character() = default;

std::string Character::Name(charenums::NameType type){
    switch(type){
        case charenums::NameType::Name:  return identity.name;
        case charenums::NameType::Color: return identity.color;
        case charenums::NameType::Both:  return std::format("{}{}{}", identity.color, identity.name, identity.color.empty() ? "" : "\x1b[0m");
    }
}
void Character::Name(std::string str, charenums::NameType nt){
    switch(nt){
        case charenums::NameType::Name:  
            identity.name = str;
            break;
        case charenums::NameType::Color: 
            identity.color = str;
            break;
        case charenums::NameType::Both:  
            identity.color = str;
            identity.name = str;
            break;
    }
}

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