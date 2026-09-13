#include "../../header/CharacterType/Sorcerer.hpp"

const ReverseCTSystem& Sorcerer::RCTSystem() const noexcept{
    return rct_system;
}

ReverseCTSystem& Sorcerer::RCTSystem() noexcept{
    return rct_system;
}