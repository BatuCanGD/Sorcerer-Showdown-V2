#include "../../header/Systems/ResourceHandler.hpp"
#include "../../header/CharacterType/CurseUser.hpp"
#include "../../header/Systems/System/SorcerySystem.hpp"
#include "../../header/Systems/System/ShikigamiSystem.hpp"
#include "../../header/Logger.hpp"
#include "../../header/Systems/System/VowSystem.hpp"
#include "../../header/Systems/System/EffectSystem.hpp"
#include "../../header/Battlefield.hpp"

void ResourceHandler::TickAll(Battlefield& bf){
    for (auto& c : bf.battlefield){
        const auto ef = EffectSystem::ApplyEffects(*c);
        if (auto* crs = c->CanUseSorcery()){
            VowSystem::ApplyVows(*crs);
            ResourceHandler::TickCursedEnergy(*crs);
            ResourceHandler::TickShikigami(*crs);
            ResourceHandler::TickRCT(*crs);
        }
        Log::Effects(ef);
    }
}

void ResourceHandler::TickCursedEnergy(CurseUser& curse_user){
    curse_user.CursedEnergy().cursed_energy += curse_user.CursedEnergy().regeneration_amount;
}
void ResourceHandler::TickShikigami(CurseUser& curse_user){
    for (auto& c : curse_user.Jujutsu().shikigami){
        ShikigamiSystem::TickShikigami(c, curse_user);
    }
}
void ResourceHandler::TickRCT(CurseUser& curse_user) {
    if (!curse_user.RCTSystem().can_use_rct) return;
    const double& output = curse_user.RCTSystem().rct_output;
    curse_user.CursedEnergy().cursed_energy -= SorcerySystem::ApplySpendingMultiplier(SorcerySystem::ApplyRCTCost(output), curse_user);
    curse_user.State().health += output;
}