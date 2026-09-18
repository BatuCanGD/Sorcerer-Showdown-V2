#include "../../../header/Systems/System/ShikigamiSystem.hpp"
#include "../../../header/CharacterType/CurseUser.hpp"

#include <algorithm>

void ShikigamiSystem::HandleSupport(CurseUser& owner, const SupportType& st, SavedSupportValues& sv, const double mult){
    switch(st){
        case SupportType::Offense:
                if (!sv.saved){
                    sv.s_strength = owner.State().strength; 
                }
                owner.State().strength = sv.s_strength * mult;
            break;
        case SupportType::Defense:
                if (!sv.saved){
                    sv.s_durability = owner.State().durability; 
                }
                owner.State().durability = sv.s_durability * mult;
            break;
        case SupportType::Output:
            if (auto* crs = owner.CanUseSorcery()){
                if (!sv.saved){
                    sv.s_output = crs->Output().max_output_potential; 
                }
                crs->Output().max_output_potential = sv.s_output * mult;
            }
            break;
        case SupportType::CursedEnergyRegen:
            if (auto* crs = owner.CanUseSorcery()){
                if (!sv.saved){
                    sv.s_ce_regen = crs->Sorcery().regeneration_amount; 
                }
                crs->Sorcery().regeneration_amount = sv.s_ce_regen * mult;
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
            if (!sk.saved_vals.undone){
                ShikigamiSystem::HandleShadow(owner, sk.support_type, sk.saved_vals); // case 1: if user changes target the values get carried to the target
                sk.saved_vals.undone = true;
            }
            return;
        }
        if (sk.saved_vals.undone){
            sk.saved_vals.undone = false;
        }
    }
    const double mult = sk.summon_type == SummonType::Support ? 1.25 : 2.0;
    ShikigamiSystem::HandleSupport(owner, sk.support_type, sk.saved_vals, mult);
}

void ShikigamiSystem::HandleShadow(CurseUser& owner, const SupportType& st, const SavedSupportValues& sv){
    switch(st){
        case SupportType::Offense:
            owner.State().strength = sv.s_strength;
            break;
        case SupportType::Defense:
            owner.State().durability = sv.s_durability;
            break;
        case SupportType::Output:
            if (auto* crs = owner.CanUseSorcery()){
                crs->Output().max_output_potential = sv.s_output;
            }
            break;
        case SupportType::CursedEnergyRegen:
            if (auto* crs = owner.CanUseSorcery()){
                crs->Sorcery().regeneration_amount = sv.s_ce_regen;
            }
            break;
    }
}