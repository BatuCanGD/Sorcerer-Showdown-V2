#include "../../header/Systems/ResourceHandler.hpp"
#include "../../header/CharacterType/CurseUser.hpp"
#include "../../header/Systems/System/SorcerySystem.hpp"
#include "../../header/Systems/System/ShikigamiSystem.hpp"
#include "../../header/Battlefield.hpp"

#include <vector>

void ResourceHandler::TickAll(Battlefield& bf){
    for (auto& c : bf.battlefield){
        ResourceHandler::TickStatusEffects(c->State().status_effects);
        if (auto* crs = c->CanUseSorcery()){
            ResourceHandler::TickCursedEnergy(*crs);
            ResourceHandler::TickShikigami(*crs);
            ResourceHandler::TickRCT(*crs);
        }
    }
}

void ResourceHandler::TickStatusEffects(std::vector<StatusEffect> &ste){
    for (auto& c : ste){
        c.turn_amount--;
    }
    std::erase_if(ste, [&](const auto& c){
        return c.turn_amount <= 0;
    });
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
    curse_user.CursedEnergy().cursed_energy -= SorcerySystem::ApplyRCTCost(output);
    curse_user.State().health += output;
}





void ResourceHandler::SpendNeutralizerCost(CurseUser& curse_user){
    curse_user.CursedEnergy().cursed_energy -= curse_user.Jujutsu().domain_neutralizer->cost;
}
void ResourceHandler::SpendDomainCost(CurseUser& curse_user){
   curse_user.CursedEnergy().cursed_energy -= curse_user.Jujutsu().domain->cost;
}