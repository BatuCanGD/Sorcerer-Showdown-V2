#pragma once
#include "CurseUser.hpp"

struct SorcererSystem final {
    double rct_output{0.0};
    bool can_use_rct{false};
    enum class RCTLevel : std::uint8_t { Wasteful, Crude, Adept, Expert, Absolute };
};

class Sorcerer final : public CurseUser {
    friend struct CharacterEditor;
    friend struct CombatSystem;
protected:
    SorcererSystem rct_system;
    SorcererSystem::RCTLevel rct_level = SorcererSystem::RCTLevel::Adept;
public:
    void ReverseCursedTechnique(double amount);
    void ReverseCursedTechnique(SorcererSystem::RCTLevel type);
};