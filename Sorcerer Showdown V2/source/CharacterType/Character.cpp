#include "../../header/CharacterType/Character.hpp"
#include "../../header/Systems/System/CombatSystem.hpp"

#include <format>
#include <stdexcept>

Character::~Character() = default;


const EntityId& Character::Identity() const noexcept {
    return identity;
}
const CharState& Character::State() const noexcept {
    return state;
}
const CharInv& Character::Equipment() const noexcept {
    return equipment;
}
const CharCtrl& Character::Control() const noexcept {
    return control;
}

std::string Character::Name(charenums::NameType type) const noexcept {
    switch(type){
        case charenums::NameType::Name:  return identity.name;
        case charenums::NameType::Color: return identity.color;
        case charenums::NameType::Both:  return identity.color.empty() ? identity.name : std::format("{}{}\x1b[0m", identity.color, identity.name);
    }
    return "";
}

double Character::Health(type::Get type) const noexcept {
    switch (type) {
        case type::Get::Current:  return state.health;
        case type::Get::Max:      return state.max_health;
    }
    return -1.0;
}

void Character::Health(type::Type type, double amount) {
    if (amount <= 0.0){
        throw std::range_error("Cannot use 0 or negative amounts");
    }
    switch (type) {
        case type::Type::Add:  
            state.health += amount;
            break;
        case type::Type::Expend:
            state.health -= amount;
            break;
        case type::Type::Set:
            state.health = amount;
            break;
        case type::Type::Get:
            throw std::invalid_argument("This overload function is not a getter");
    }
    if (state.health > state.max_health){
        state.health = state.max_health;
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

const CurseUser* Character::CanUseSorcery() const noexcept {
    return nullptr;
}