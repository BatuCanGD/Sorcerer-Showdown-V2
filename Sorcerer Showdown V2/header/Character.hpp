#pragma once
#include "Info.hpp"
#include "BaseStruct.hpp"
#include "SorceryStruct.hpp"

struct Character final {
    std::optional<Sorcery::CurseUserSystem> curseUser;
    Base::Equipment equipment;
    EntityInfo identity;
    Base::State state;
    Base::DecisionMaker brain;
};