#pragma once
#include "Sorcery/Technique.hpp"
#include "Sorcery/Domain.hpp"
#include "Sorcery/Shikigami.hpp"
#include "Sorcery/BindingVow.hpp"
#include "Sorcery/Neutralizer.hpp"

struct BindingVow;
struct Shikigami;

namespace Sorcery {

    struct JujutsuSystem final {
        std::vector<BindingVow> binding_vows;
        std::vector<Shikigami> shikigami;
        std::optional<Technique> technique;
        std::optional<Domain> domain;
        std::optional<Neutralizer> neutralizer;
    };

    struct AmplificationSystem final {
        bool can_use_amp{false};
        bool is_active{false};
    };

    struct OutputSystem final {
        double max_output_potential{100.0};
        double current_output{0.0};
        double output_cooldown_amount{1.0};
        std::uint8_t normalize_tick{0};
        enum class Status : std::uint8_t { Regular, BurntOut, Boosted, Max };
        Status status{Status::Regular};
    };

    struct CursedEnergySystem final {
        double cursed_energy{1.0};
        double max_cursed_energy{1.0};
        double regeneration_amount{1.0};
        double reinforcement_amount{0.0};
        std::uint8_t bf_chance{1}; 
        enum class Efficiency : std::uint8_t { Wasteful, Rough, Unstable, Stable, Precise, Expert, Extreme, Peak };
        Efficiency efficiency{Efficiency::Stable};
    };

    struct SorceryTraits final {
        bool six_eyes{false};
        bool passive_healing{false};
    };

    struct RCTSystem final {
        double rct_output{0.0};
        bool can_use_rct{false};
    };

    struct CurseUserSystem final {
        Sorcery::JujutsuSystem jujutsu;
        Sorcery::CursedEnergySystem ce_system;
        Sorcery::OutputSystem output;
        Sorcery::AmplificationSystem amplification;
        Sorcery::RCTSystem rct_system;
        Sorcery::SorceryTraits traits;
    };
}