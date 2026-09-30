#include "../../header/CharacterType/Character.hpp"
#include "../../header/Systems/System/CombatSystem.hpp"

Character::Character() {};
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
const BattleIQ& Character::Style() const noexcept {
    return style;
}
BattleIQ& Character::Style() noexcept {
    return style;
}

DamageStruct Character::Damage(double amount, globalums::DamageType type){
    DamageStruct ds{.attack_blocked = true};
    if (state.is_invulnerable){ // godmode/dev character check
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

CurseUser* Character::CanUseSorcery() noexcept {
    return nullptr;
}

std::unique_ptr<Character> Character::Clone() const {
    return std::make_unique<Character>(*this);
}