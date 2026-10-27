#pragma once
#include "../../Sorcery/Shikigami.hpp"
#include "CombatSystem.hpp"

struct Character;


struct DamageStruct;

namespace ShikigamiSystem {
    DamageStruct Damage(Shikigami& s, double amount, DamageType dt = DamageType::Normal);
    void TickShikigami(Shikigami& sk, Character& owner);
    void HandleSupport(Character& owner, const SupportType& st, SavedValue& saved_vals, const double mult);
    void HandleShadow(Character& sk, const SupportType& st, const SavedValue& sv);
}