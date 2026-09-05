#pragma once

#include "../Enums.hpp"

#include <string>
#include <memory>
#include <vector>

struct AttackStruct;

struct CharIdentity final {
    std::string name{""};
    std::string color{""};
};

struct CharState final {
    double health{1.0};
    double max_health{1.0};
    double previous_health{1.0};
    double durability{1.0};
    double strength{1.0};
    bool is_stunned{false};
    bool is_invulnerable{false};
};

struct CharInv final {
    std::vector<std::unique_ptr<int>> inventory; // placeholder ints
    std::unique_ptr<int> stored_tool{nullptr};
    std::unique_ptr<int> current_tool{nullptr};
    bool has_access_to_inventory{false};
};

class Character {
    friend struct Editor;
    friend struct Helper;
    friend struct CombatHelper;
protected:
    CharIdentity identity;
    CharState state;
    CharInv equipment;
public:
    Character() {};
    virtual ~Character();

    double Health(type::Get type = type::Get::Current) const noexcept;
    void Health(type::Set type, double amount);

    void Damage(double amount, globalums::DamageType dmg_type = globalums::DamageType::Normal);
    AttackStruct Attack(Character& attacked);
};