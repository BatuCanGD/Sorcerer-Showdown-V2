#pragma once
#include "Character.hpp"

#include "../Sorcery/Shikigami.hpp"
#include "../Sorcery/BindingVow.hpp"
#include "../Sorcery/Technique.hpp"
#include "../Sorcery/Domain.hpp"
#include "../Sorcery/Neutralizer.hpp"

struct JujutsuSystem final {
    std::vector<BindingVow> binding_vows;
    std::vector<Shikigami> shikigami;
    std::optional<Technique> technique{};
    std::optional<Domain> domain{};
    std::optional<Neutralizer> neutralizer{};
};

struct CurseUserAmplification final {
    bool is_usable{false};
    bool is_active{false};
};

struct CurseUserOutput final {
    double max_output_potential{100.0};
    double current_output{0.0};
};

struct CursedEnergySystem final {
    double cursed_energy{1.0};
    double max_cursed_energy{1.0};
    double regeneration_amount{1.0};
    double reinforcement_amount{1.0};
    std::uint8_t bf_chance{1}; 
    enum class Efficiency : std::uint8_t { Wasteful, Rough, Unstable, Stable, Expert, Extreme, Ultimate, Absolute };
    Efficiency efficiency{Efficiency::Stable};
};

struct SorceryTrait final {
    bool six_eyes{false};
    bool passive_healing{false};
};

struct ReverseCTSystem final {
    double rct_output{0.0};
    bool can_use_rct{false};
    enum class RCTLevel : std::uint8_t { Wasteful, Crude, Adept, Expert, Absolute };
    RCTLevel rct_level{RCTLevel::Adept};
};

class CurseUser final : public Character {
    JujutsuSystem jujutsu;
    CursedEnergySystem ce_system;
    CurseUserOutput output;
    CurseUserAmplification amplification;
    ReverseCTSystem rct_system;
    SorceryTrait traits;
public:
    CurseUser();
    ~CurseUser() override;
    
    [[nodiscard]] const JujutsuSystem& Jujutsu() const noexcept;
    [[nodiscard]] const CursedEnergySystem& CursedEnergy() const noexcept;
    [[nodiscard]] const CurseUserOutput& Output() const noexcept;
    [[nodiscard]] const CurseUserAmplification& Amplification() const noexcept;
    [[nodiscard]] const SorceryTrait& Traits() const noexcept;
    [[nodiscard]] const ReverseCTSystem& RCTSystem() const noexcept;

    JujutsuSystem& Jujutsu() noexcept;
    CursedEnergySystem& CursedEnergy() noexcept;
    CurseUserOutput& Output() noexcept;
    CurseUserAmplification& Amplification() noexcept;
    SorceryTrait& Traits() noexcept;
    ReverseCTSystem& RCTSystem() noexcept;
    
    [[nodiscard]] const CurseUser* CanUseSorcery() const noexcept override;
    CurseUser* CanUseSorcery() noexcept override;

    [[nodiscard]] std::unique_ptr<Character> Clone() const override;
};