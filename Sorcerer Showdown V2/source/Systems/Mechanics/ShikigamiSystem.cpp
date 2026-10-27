#include "../../../header/Systems/Mechanics/ShikigamiSystem.hpp"
#include "../../../header/Character.hpp"
#include <algorithm>

DamageStruct ShikigamiSystem::Damage(Shikigami &s, double amount, DamageType dt) {
    DamageStruct ds{CombatSystem::ResolveDamage(&s, dt, amount)};
    if (ds.attack_blocked){
        return {.attack_blocked = true};
    }
    s.stats.health -= ds.damage;
    return ds;
}

void ShikigamiSystem::HandleSupport(Character& owner, const SupportType& st, SavedValue& sv, const double mult){
    const double amount = sv.value * mult;
    
    switch(st){
        case SupportType::Offense:
                if (!sv.saved){
                    sv.value = owner.state.strength; 
                }
                owner.state.strength = amount;
            break;
        case SupportType::Defense:
                if (!sv.saved){
                    sv.value = owner.state.durability; 
                }
                owner.state.durability = amount;
            break;
        case SupportType::Output:
            if (auto& crs = owner.curseUser){
                if (!sv.saved){
                    sv.value = crs->output.max_output_potential; 
                }
                crs->output.max_output_potential = amount;
            }
            break;
        case SupportType::CursedEnergyRegen:
            if (auto& crs = owner.curseUser){
                if (!sv.saved){
                    sv.value = crs->ce_system.regeneration_amount; 
                }
                crs->ce_system.regeneration_amount = amount;
            }
            break;
        default:
            break;
    }
    sv.saved = true;
}


void ShikigamiSystem::TickShikigami(Shikigami& sk, Character& owner) {
    if (sk.stats.health <= 0.0){
        return;
    }
    if (sk.summon_type != SummonType::Active){
        sk.stats.health = std::min(sk.stats.health + sk.stats.regen_speed, sk.stats.max_health);
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

void ShikigamiSystem::HandleShadow(Character& owner, const SupportType& st, const SavedValue& sv){
    switch(st){
        case SupportType::Offense:
            owner.state.strength = sv.value;
            break;
        case SupportType::Defense:
            owner.state.durability = sv.value;
            break;
        case SupportType::Output:
            if (auto& crs = owner.curseUser){
                crs->output.max_output_potential = sv.value;
            }
            break;
        case SupportType::CursedEnergyRegen:
            if (auto& crs = owner.curseUser){
                crs->ce_system.regeneration_amount = sv.value;
            }
            break;
        default:
            break;
    }
}