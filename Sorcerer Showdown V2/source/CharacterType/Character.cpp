#include "../../header/CharacterType/Character.hpp"
#include "../../header/Systems/Fighting/CombatSystem.hpp"

#include <format>

Character::~Character() = default;


const EntityId& Character::Identity() const {
    return identity;
}
const CharState& Character::State() const noexcept {
    return state;
}
const CharInv& Character::Equipment() const {
    return equipment;
}
const CharCtrl& Character::Control() const noexcept{
    return control;
}

std::string Character::Name(charenums::NameType type){
    switch(type){
        case charenums::NameType::Name:  return identity.name;
        case charenums::NameType::Color: return identity.color;
        case charenums::NameType::Both:  return identity.color.empty() ? identity.name : std::format("{}{}\x1b[0m", identity.color, identity.name);
    }
    return "";
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
            identity.color  = str;
            identity.name   = str;
            break;
    }
}

double Character::Health(type::Get type) const noexcept {
    switch (type) {
        case type::Get::Current:  return state.health;
        case type::Get::Max:      return state.max_health;
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
    }
}
void Character::Health(type::Expend type, double amount) {
        switch (type) {
        case type::Expend::Current:  
            state.health -= amount;
            break;
        case type::Expend::Max:
            state.max_health -= amount;
            break;
    }
}
void Character::Health(type::Add type, double amount){
    switch (type) {
        case type::Add::Current:  
            state.health += amount;
            break;
        case type::Add::Max:
            state.max_health += amount;
            break;
    }
}

DamageStruct Character::Damage(double amount, globalums::DamageType type){
    if (state.is_invulnerable){
        return {.attack_blocked = true};
    }
    const auto& ds = CombatSystem::ResolveDamage(*this, type, amount);
    if (ds.attack_blocked){
        return ds;
    }
    this->state.health -= ds.damage;
    return ds;
}
AttackStruct Character::Attack(Character& cc){
    return CombatSystem::ResolveAttacking(*this, cc);
}

bool Character::CanUseSorcery() const {
    return false;
}