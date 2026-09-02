#pragma once
#include "CurseUser.hpp"

struct SorcererSystem final {
    double rct_output{0.0};
    bool can_use_rct{false};
};

class Sorcerer : public CurseUser {
    friend struct CharacterEditor;
public:
    enum class RCTLevel : std::uint8_t { Wasteful, Crude, Adept, Expert, Absolute };
protected:
    SorcererSystem sorcerery;
    RCTLevel rct_level = RCTLevel::Adept;
public:
    void ReverseCursedTechnique(double amount);
    void ReverseCursedTechnique(RCTLevel type);
};