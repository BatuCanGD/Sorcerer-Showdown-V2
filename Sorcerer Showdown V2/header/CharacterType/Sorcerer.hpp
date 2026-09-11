#pragma once
#include "CurseUser.hpp"

struct ReverseCTSystem final {
    double rct_output{0.0};
    bool can_use_rct{false};
    enum class RCTLevel : std::uint8_t { Wasteful, Crude, Adept, Expert, Absolute };
    RCTLevel rct_level{RCTLevel::Adept};
};

class Sorcerer final : public CurseUser {
    friend struct CharacterEditor;
    friend struct CombatSystem;
protected:
    ReverseCTSystem rct_system;
public:
    [[nodiscard]] const ReverseCTSystem& RCTSystem() const noexcept;
};