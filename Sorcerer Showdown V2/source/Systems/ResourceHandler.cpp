#include "../../header/Systems/ResourceHandler.hpp"
#include "../../header/CharacterType/CurseUser.hpp"
#include "../../header/Systems/System/ShikigamiSystem.hpp"
#include <vector>

void ResourceHandler::TickStatusEffects(std::vector<StatusEffect> &ste){
    for (auto& c : ste){
        c.turn_amount--;
    }
    std::erase_if(ste, [&](const auto& c){
        return c.turn_amount <= 0;
    });
}

void ResourceHandler::TickCursedEnergy(CurseUser& curse_user){
    curse_user.CursedEnergy(OpType::Add, curse_user.Sorcery().regeneration_amount);
}
void ResourceHandler::TickShikigami(CurseUser& curse_user){
    for (auto& c : curse_user.Jujutsu().shikigami){
        ShikigamiSystem::TickShikigami(c, curse_user);
    }
}
void ResourceHandler::SpendNeutralizerCost(CurseUser& curse_user){
    curse_user.CursedEnergy(OpType::Subtract, curse_user.Jujutsu().domain_neutralizer->cost);
}
void ResourceHandler::SpendDomainCost(CurseUser& curse_user){
    curse_user.CursedEnergy(OpType::Subtract, curse_user.Jujutsu().domain->cost);
}
void ResourceHandler::TickRCT(CurseUser& curse_user) {
    if (!curse_user.RCTSystem().can_use_rct) return;
    curse_user.CursedEnergy(OpType::Subtract, curse_user.RCTSystem().rct_output); // add a function that multiplies output with rct level
    curse_user.Health(OpType::Add, curse_user.RCTSystem().rct_output);
}
