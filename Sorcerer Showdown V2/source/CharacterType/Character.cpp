#include "../../header/CharacterType/Character.hpp"
#include "../../header/Systems/System/CombatSystem.hpp"

#include <stdexcept>

Character::~Character() = default;


const EntityInfo& Character::Identity() const noexcept {
    return identity;
}
EntityInfo& Character::Identity() noexcept {
    return identity;
}
const CharState& Character::State() const noexcept {
    return state;
}
CharState& Character::State() noexcept {
    return state;
}
const CharInv& Character::Equipment() const noexcept {
    return equipment;
}
CharInv& Character::Equipment() noexcept {
    return equipment;
}
const CharCtrl& Character::Control() const noexcept {
    return control;
}
CharCtrl& Character::Control() noexcept {
    return control;
}

std::string Character::Name() const noexcept {
    return identity.color + identity.name + (identity.color.empty() ? "" : "\x1b[0m");
}

double Character::Health(ValType type) const noexcept {
    switch (type) {
        case ValType::Current:  return state.health;
        case ValType::Maximum:  return state.max_health;
    }
    return -1.0;
}

void Character::Health(OpType type, double amount) {
    if (amount <= 0.0){
        throw std::range_error("Cannot use 0 or negative amounts");
    }
    switch (type) {
        case OpType::Add:  
            state.health += amount;
            break;
        case OpType::Expend:
            state.health -= amount;
            break;
        case OpType::Set:
            state.health = amount;
            break;
        case OpType::Get:
            throw std::invalid_argument("This overload function is not a getter");
    }
    if (state.health > state.max_health){
        state.health = state.max_health;
    }
}

DamageStruct Character::Damage(double amount, globalums::DamageType type){
    DamageStruct ds{.attack_blocked = true};
    if (state.is_invulnerable){
        return ds;
    }
    ds = CombatSystem::ResolveDamage(*this, type, amount);
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