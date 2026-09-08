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

struct CharCtrl final {
    bool is_player{false};
};

struct CharState final {
    double health{1.0};
    double max_health{1.0};
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
    friend struct CharacterEditor;
    friend struct Combat;
protected:
    CharIdentity identity;
    CharCtrl control;
    CharState state;
    CharInv equipment;
public:
    Character() {};
    virtual ~Character();

    std::string Name(charenums::NameType type = charenums::NameType::Both);
    void Name(std::string str, charenums::NameType nt = charenums::NameType::Name);

    double Health(type::Get type = type::Get::Current) const noexcept;
    void Health(type::Set type, double amount);
    void Health(type::Expend t, double amount);
    void Health(type::Add type, double amount);

    void Damage(double amount, globalums::DamageType dmg_type = globalums::DamageType::Normal);
    AttackStruct Attack(Character& attacked);

    virtual bool CanUseSorcery() const;
};