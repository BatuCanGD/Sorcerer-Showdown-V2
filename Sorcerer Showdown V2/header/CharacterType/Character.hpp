#pragma once

#include <string>
#include <cstdint>
#include <memory>
#include <vector>

struct CharIdentity final {
    std::string name;
    std::string color;
};

struct CharState final {
    double health{1.0};
    double max_health{1.0};
    double previous_health{1.0};
    bool is_stunned{false};
    bool is_invulnerable{false};
};

struct CharInv final {
    std::vector<std::unique_ptr<int>> inventory; // placeholder int
    std::unique_ptr<int> current_tool; // placeholder int
};

class Character {
private:
    CharIdentity identity;
    CharState state;
    CharInv equipment;   
public:
    enum class Get : std::uint8_t { Current, Max, Previous };
    enum class Set : std::uint8_t { Current, Max, Previous };

    double Health(Get type = Get::Current) const;
    void Health(Set type, double amount);
};