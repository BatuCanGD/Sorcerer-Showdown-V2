#pragma once
#include "Character.hpp"

#include "../Stuff/Shikigami.hpp"
#include "../Stuff/BindingVow.hpp"
#include "../Sorcery/Technique.hpp"
#include "../Enums.hpp"

struct JujutsuSystem final {
    std::vector<BindingVow> binding_vows; // placeholder ints
    std::vector<Shikigami> shikigami;
    std::optional<Technique> technique{};
    std::optional<int> domain{};
    std::optional<int> domain_neutralizer{};
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
    std::uint8_t bf_chance{1}; 
    enum class Efficiency : std::uint8_t { 
        Wasteful, Rough, Unstable, Stable, 
        Expert, Extreme, Ultimate, Absolute 
    };
    Efficiency efficiency{Efficiency::Stable};
};

struct SorceryTrait final {
    bool six_eyes{false};
    bool passive_healing{false};
};

class CurseUser : public Character {
   friend struct CharacterEditor; 
   friend struct CombatSystem;
protected:
    JujutsuSystem jujutsu;
    CursedEnergySystem ce_system;
    CurseUserOutput output;
    CurseUserAmplification amplification;
    SorceryTrait traits;
public:
    [[nodiscard]] const JujutsuSystem& Jujutsu() const noexcept;
    [[nodiscard]] const CursedEnergySystem& CursedEnergySys() const noexcept;
    [[nodiscard]] const CurseUserOutput& Output() const noexcept;
    [[nodiscard]] const CurseUserAmplification& Amplification() const noexcept;
    [[nodiscard]] const SorceryTrait& Traits() const noexcept;

    JujutsuSystem& Jujutsu() noexcept;
    CursedEnergySystem& CursedEnergySys() noexcept;
    CurseUserOutput& Output() noexcept;
    CurseUserAmplification& Amplification() noexcept;
    SorceryTrait& Traits() noexcept;
    
    double CursedEnergy(type::Get type = type::Get::Current) const noexcept;
    void CursedEnergy(type::Type type, double amount);

    double Output(type::Get type) const noexcept;
    void Output(type::Type type, double amount);

    [[nodiscard]] const CurseUser* CanUseSorcery() const noexcept override;
    [[nodiscard]] bool HasSixEyes() const noexcept;
};