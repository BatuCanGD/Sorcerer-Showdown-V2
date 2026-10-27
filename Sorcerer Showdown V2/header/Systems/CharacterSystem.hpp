#pragma once
#include "../DamageType.hpp"
#include <variant>
#include <memory>

struct AttackStruct;
struct ToolStruct;
struct DamageStruct;
struct Shikigami;
struct Character;

namespace CharacterSystem {
    DamageStruct Damage(Character& damaged, double amount, DamageType dmg_type = DamageType::Normal);
    std::variant<AttackStruct, ToolStruct> Attack(Character& attacker, std::variant<Character*, Shikigami*> attacked);
    bool HPMoreThanMax(const Character& c, double d);
    std::unique_ptr<Character> Clone(const Character& c);
}