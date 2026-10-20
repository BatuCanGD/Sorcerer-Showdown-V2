#pragma once
#include "../../Sorcery/Shikigami.hpp"
#include "CombatSystem.hpp"

class Character;
class CurseUser;

struct DamageStruct;

namespace ShikigamiSystem {
    DamageStruct Damage(Shikigami& s, double amount, globalums::DamageType dt);
    void TickShikigami(Shikigami& sk, CurseUser& owner);
    void HandleSupport(CurseUser& owner, const SupportType& st, SavedValue& saved_vals, const double mult);
    void HandleShadow(CurseUser& sk, const SupportType& st, const SavedValue& sv);
}