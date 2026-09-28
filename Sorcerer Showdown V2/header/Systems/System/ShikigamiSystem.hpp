#pragma once
#include "../../Sorcery/Shikigami.hpp"

class CurseUser;

namespace ShikigamiSystem {
    void TickShikigami(Shikigami& sk, CurseUser& owner);
    void HandleSupport(CurseUser& owner, const SupportType& st, SavedValue& saved_vals, const double mult);
    void HandleShadow(CurseUser& sk, const SupportType& st, const SavedValue& sv);
}