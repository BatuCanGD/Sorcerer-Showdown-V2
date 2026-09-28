#include "../../../header/Systems/System/ShikigamiSystem.hpp"
#include "../../../header/CharacterType/CurseUser.hpp"

#include <algorithm>

void ShikigamiSystem::HandleSupport(CurseUser& owner, const SupportType& st, SavedValue& sv, const double mult){
    switch(st){
        case SupportType::Offense:
                if (!sv.saved){
                    sv.value = owner.State().strength; 
                }
                owner.State().strength = sv.value * mult;
            break;
        case SupportType::Defense:
                if (!sv.saved){
                    sv.value = owner.State().durability; 
                }
                owner.State().durability = sv.value * mult;
            break;
        case SupportType::Output:
            if (auto* crs = owner.CanUseSorcery()){
                if (!sv.saved){
                    sv.value = crs->Output().max_output_potential; 
                }
                crs->Output().max_output_potential = sv.value * mult;
            }
            break;
        case SupportType::CursedEnergyRegen:
            if (auto* crs = owner.CanUseSorcery()){
                if (!sv.saved){
                    sv.value = crs->CursedEnergy().regeneration_amount; 
                }
                crs->CursedEnergy().regeneration_amount = sv.value * mult;
            }
            break;
    }
    sv.saved = true;
}


void ShikigamiSystem::TickShikigami(Shikigami& sk, CurseUser& owner) {
    if (sk.hp.health <= 0.0){
        return;
    }
    if (sk.summon_type != SummonType::Active){
        sk.hp.health = std::min(sk.hp.health + sk.hp.regen_speed, sk.hp.max_health);
        if (sk.summon_type == SummonType::Shadow){
            if (!sk.rollback.undone){
                ShikigamiSystem::HandleShadow(owner, sk.support_type, sk.rollback);
                sk.rollback.undone = true;
            }
            return;
        }
    }
    if (sk.rollback.undone){
        sk.rollback.undone = false;
    }
    const double mult = sk.summon_type == SummonType::Support ? 1.25 : 2.0;
    ShikigamiSystem::HandleSupport(owner, sk.support_type, sk.rollback, mult);
}

void ShikigamiSystem::HandleShadow(CurseUser& owner, const SupportType& st, const SavedValue& sv){
    switch(st){
        case SupportType::Offense:
            owner.State().strength = sv.value;
            break;
        case SupportType::Defense:
            owner.State().durability = sv.value;
            break;
        case SupportType::Output:
            if (auto* crs = owner.CanUseSorcery()){
                crs->Output().max_output_potential = sv.value;
            }
            break;
        case SupportType::CursedEnergyRegen:
            if (auto* crs = owner.CanUseSorcery()){
                crs->CursedEnergy().regeneration_amount = sv.value;
            }
            break;
    }
}