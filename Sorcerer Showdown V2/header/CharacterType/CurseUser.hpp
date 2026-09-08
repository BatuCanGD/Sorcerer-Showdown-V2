#pragma once
#include "Character.hpp"
#include "../Sorcery/Technique.hpp"
#include "../Enums.hpp"

#include <memory>

struct Jujutsu final {
    std::vector<std::unique_ptr<int>> binding_vows; // placeholder ints
    std::vector<std::unique_ptr<int>> shikigami;
    std::unique_ptr<Technique> technique{nullptr};
    std::unique_ptr<int> domain{nullptr};
    std::unique_ptr<int> domain_neutralizer{nullptr};
};

struct CurseUserSystem final {
    double max_output_potential{100.0};
    double current_output{0.0};
    double cursed_energy{1.0};
    double max_cursed_energy{1.0};
    int bf_chance{15};
    bool can_use_amplification{false};
    bool amplification_is_active{false};
};

struct SorceryTrait final {
    bool six_eyes{false};
    bool passive_healing{false};
};

class CurseUser : public Character {
   friend struct CharacterEditor; 
   friend struct Combat;
public:
    enum class CEfficiency : std::uint8_t { Wasteful, Rough, Unstable, Stable, Expert, Extreme, Ultimate, Absolute };
protected:
    Jujutsu jujutsu;
    CurseUserSystem sorcery;
    SorceryTrait traits;
    CEfficiency ce_efficiency = CEfficiency::Stable;
public:
    double CursedEnergy(type::Get type = type::Get::Current) const noexcept;
    void CursedEnergy(type::Set type, double amount);
    void CursedEnergy(type::Expend t, double amount);
    void CursedEnergy(type::Add type, double amount);

    double Output(type::Get t = type::Get::Current) const noexcept;
    double Output(type::Type t, double amount = 0.0);
    void Output(type::Set t, double amount); 

    void Efficiency(CEfficiency type);
    CEfficiency Efficiency() const noexcept;

    Technique* technique() const;

    bool CanUseSorcery() const override;
    bool HasSixEyes() const noexcept;
};