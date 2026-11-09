#include "../../../header/Systems/Mechanics/ShikigamiSystem.hpp"
#include "../../../header/Systems/Mechanics/SorcerySystem.hpp"
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
    if (!sv.saved){
        switch(st){
            case SupportType::Offense:
                sv.value = owner.state.strength;
                break;
            case SupportType::Defense:
                sv.value = owner.state.durability;
                break;
            case SupportType::Output:
                if (auto& crs = owner.curseUser){
                    sv.value = crs->output.max_output_potential;
                }
                break;
            case SupportType::CursedEnergyRegen:
                if (auto& crs = owner.curseUser) {
                    sv.value = crs->ce_system.regeneration_amount;
                }
                break;
            default:
                break;
        }
        sv.saved = true;
    }
    const double amount = sv.value * mult;
    if (sv.saved){
        switch(st){
            case SupportType::Offense:
                    owner.state.strength = amount;
                break;
            case SupportType::Defense:
                    owner.state.durability = amount;
                break;
            case SupportType::Output:
                if (auto& crs = owner.curseUser){
                    crs->output.max_output_potential = amount;
                }
                break;
            case SupportType::CursedEnergyRegen:
                if (auto& crs = owner.curseUser) {
                    crs->ce_system.regeneration_amount = amount;
                }
                break;
            default:
                break;
        }
    }
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
    const double mult = sk.summon_type == SummonType::Support ? 1.25 : 2.0;
    const double cost = SorcerySystem::ApplySpendingMultiplier(sk.cost * mult - 1.0, *owner.curseUser);
    auto& crs = owner.curseUser->ce_system;

    if (crs.cursed_energy < cost) {
        if (!sk.rollback.undone){
            ShikigamiSystem::HandleShadow(owner, sk.support_type, sk.rollback);
            sk.rollback.undone = true;
        }
        sk.summon_type = SummonType::Shadow;
        return;
    }
    crs.cursed_energy = std::max(crs.cursed_energy - cost, 0.0);

    if (sk.rollback.undone){
        sk.rollback.undone = false;
    }
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