#pragma once
#include "Character.hpp"

#include <memory>

struct Jujutsu final {
    std::vector<std::unique_ptr<int>> binding_vows; // placeholder ints
    std::vector<std::unique_ptr<int>> shikigami;
    std::unique_ptr<int> technique{nullptr};
    std::unique_ptr<int> domain{nullptr};
    std::unique_ptr<int> domain_neutralizer{nullptr};
};

struct CurseUserSystem final {
    double max_output_potential{100.0};
    double cursed_energy{1.0};
    double max_cursed_energy{1.0};
    double previous_cursed_energy{1.0};
    bool can_use_amplification{false};
    bool amplification_is_active{false};
};

struct SorceryTrait final {
    bool six_eyes{false};
};

class CurseUser : public Character {
public:
    enum class CEfficiency : std::uint8_t { Wasteful, Rough, Unstable, Stable, Expert, Ultimate, Extreme };
protected:
    Jujutsu jujutsu;
    CurseUserSystem sorcery;
    SorceryTrait traits;
    CEfficiency ce_efficiency = CEfficiency::Stable;
public:
    double CursedEnergy(Get type = Get::Current) const noexcept;
    void CursedEnergy(Set type, double amount);

    void Efficiency(CEfficiency type);
};