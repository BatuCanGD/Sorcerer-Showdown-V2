#include "../../header/Systems/ResourceHandler.hpp"
#include "../../header/CharacterType/CurseUser.hpp"
#include "../../header/Systems/System/SorcerySystem.hpp"
#include "../../header/Systems/System/ShikigamiSystem.hpp"
#include "../../header/Logger.hpp"
#include "../../header/Systems/System/VowSystem.hpp"
#include "../../header/Systems/System/EffectSystem.hpp"
#include "../../header/Battlefield.hpp"

#include <algorithm>

void ResourceHandler::TickAll(Battlefield& bf){
    for (auto& c : bf.battlefield){
        const auto ef = EffectSystem::ApplyEffects(*c);
        if (auto* crs = c->CanUseSorcery()){
            VowSystem::ApplyVows(*crs);
            ResourceHandler::TickCursedEnergy(*crs);
            ResourceHandler::TickShikigami(*crs);
            ResourceHandler::TickRCT(*crs);
        }
        Log::Effects(ef.first, ef.second);
    }
}

void ResourceHandler::TickCursedEnergy(CurseUser& curse_user){
    curse_user.CursedEnergy().cursed_energy = std::min(curse_user.CursedEnergy().cursed_energy + curse_user.CursedEnergy().regeneration_amount, curse_user.CursedEnergy().max_cursed_energy);
}
void ResourceHandler::TickShikigami(CurseUser& curse_user){
    for (auto& c : curse_user.Jujutsu().shikigami){
        ShikigamiSystem::TickShikigami(c, curse_user);
    }
}
void ResourceHandler::TickRCT(CurseUser& curse_user) {
    if (!curse_user.RCTSystem().can_use_rct) return;
    const double& output = curse_user.RCTSystem().rct_output;
    curse_user.CursedEnergy().cursed_energy = std::clamp(curse_user.CursedEnergy().cursed_energy - SorcerySystem::ApplySpendingMultiplier(SorcerySystem::ApplyRCTCost(output), curse_user), 0.0, curse_user.CursedEnergy().max_cursed_energy);
    curse_user.State().health = std::min(curse_user.State().health + output, curse_user.State().max_health);
}