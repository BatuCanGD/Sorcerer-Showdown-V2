#pragma once

#include <string>
#include <cstdint>
#include <memory>
#include <vector>

namespace enumtype {
    enum class DamageType {
        Normal, // can be stopped or negated by reinforcement
        BypassTech, // techniques, infinity, etc...
        BypassRein, // Cursed Energy Reinforcement
        BypassAll // all of the above
    };
}

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
protected:
    CharIdentity identity;
    CharState state;
    CharInv equipment;
public:
    Character() {};
    virtual ~Character();
    enum class Get : std::uint8_t { Current, Max, Previous };
    enum class Set : std::uint8_t { Current, Max, Previous };

    double Health(Get type = Get::Current) const noexcept;
    void Health(Set type, double amount);

    void Damage(double amount, enumtype::DamageType dmg_type = enumtype::DamageType::Normal);
    void Attack(Character& attacked);
};