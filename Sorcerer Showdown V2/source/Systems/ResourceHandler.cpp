#include "../../header/Systems/ResourceHandler.hpp"
#include "../../header/CharacterType/CurseUser.hpp"

void ResourceHandler::TickCursedEnergy(CurseUser& curse_user){
    curse_user.CursedEnergy(OpType::Add, curse_user.CursedEnergySys().regeneration_amount);
}
void ResourceHandler::TickShikigami(CurseUser& curse_user){
    for ([[maybe_unused]] const auto& c : curse_user.Jujutsu().shikigami){
        // add a shikigami tick function
    }
}
void ResourceHandler::UsedNeutralizer(CurseUser& curse_user){
    curse_user.CursedEnergy(OpType::Expend, curse_user.Jujutsu().domain_neutralizer->cost);
}
void ResourceHandler::UsedDomain(CurseUser& curse_user){
    curse_user.CursedEnergy(OpType::Expend, curse_user.Jujutsu().domain->cost);
}
void ResourceHandler::TickRCT(CurseUser& curse_user) {
    if (!curse_user.RCTSystem().can_use_rct) return;
    curse_user.CursedEnergy(OpType::Expend, curse_user.RCTSystem().rct_output); // add a function that multiplies output with rct level
    curse_user.Health(OpType::Add, curse_user.RCTSystem().rct_output);
}
