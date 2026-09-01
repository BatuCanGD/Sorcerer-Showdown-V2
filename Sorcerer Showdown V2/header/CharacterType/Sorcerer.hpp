#pragma once
#include "CurseUser.hpp"

struct Sorcerery final {
    double rct_output{0.0};
    bool can_use_rct{false};
    
};

class Sorcerer : public CurseUser {
public:
    enum class RCTLevel : std::uint8_t { Wasteful, Crude, Adept, Expert, Absolute };
protected:
    Sorcerery sorcerery;
    RCTLevel rct_level = RCTLevel::Adept;
public:
};