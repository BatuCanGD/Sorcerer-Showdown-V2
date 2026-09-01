#pragma once
#include "Character.hpp"

#include <memory>

struct CharSorcery final {
    std::vector<std::unique_ptr<int>> binding_vows;
    std::vector<std::unique_ptr<int>> shikigami;
    std::unique_ptr<int> technique;
    std::unique_ptr<int> domain;
    std::unique_ptr<int> domain_neutralizer;
};

struct SorcerySystem final {
    double cursed_energy{1.0};
    double max_cursed_energy{1.0};
    double previous_cursed_energy{1.0};

    bool can_use_amplification{false};
    bool amplification_is_active{false};
};

class CurseUser : public Character {
    CharSorcery jujutsu;
    SorcerySystem sorcery;
public:
    enum class CEfficiency : std::uint8_t { Wasteful, Rough, Unstable, Stable, Expert, Ultimate, Extreme };

    double CursedEnergy(Get type = Get::Current) const;
    void CursedEnergy(Set type, double amount);

    CEfficiency Efficiency() const;
    void Efficiency(CEfficiency type);
private:
    CEfficiency ce_efficiency = CEfficiency::Stable;
};