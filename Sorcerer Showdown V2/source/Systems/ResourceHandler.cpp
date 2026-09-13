#include "../../header/Systems/ResourceHandler.hpp"
#include "../../header/CharacterType/Sorcerer.hpp"



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
void ResourceHandler::TickReverseCursedTechnique(Sorcerer& sorcerer){
    if (!sorcerer.RCTSystem().can_use_rct) return;
    sorcerer.CursedEnergy(OpType::Expend, sorcerer.RCTSystem().rct_output); // add a function that multiplies output with rct level
    sorcerer.Health(OpType::Add, sorcerer.RCTSystem().rct_output);
}
