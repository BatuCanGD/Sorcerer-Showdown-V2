#include "../../../header/Systems/System/DomainSystem.hpp"
#include "../../../header/Sorcery/Domain.hpp"
#include "../../../header/CharacterType/CurseUser.hpp"
#include "../../../header/Systems/System/CombatSystem.hpp"

#include <stdexcept>


double DomainSystem::CalculateHitDamage(const Domain& domain, const Character& c) {
    double damage{domain.damage};
    if (const auto* cc = c.CanUseSorcery()) { 
        if (const auto& nl = cc->Jujutsu().neutralizer){
            if (nl->is_active && nl->neutralizer_type == NeutralizerType::ReducedDamage){
                damage /= 2.5;
            }
        }
    }
    return damage;
}
bool DomainSystem::CalculateActualHit(const Domain& domain, const Character& c) {
    if (const auto* cc = c.CanUseSorcery()) { 
        if (const auto& nl = cc->Jujutsu().neutralizer){
            if (nl->is_active){
                return nl->neutralizer_type == NeutralizerType::ReducedDamage && domain.surehit_type != SurehitType::Basic;
            }
        }
    }
    return true;
}

DomainClashStruct DomainSystem::ClashDomains(Domain& first, Domain& second) {
    if (!(first.is_active && second.is_active)) {
        throw std::invalid_argument("Both domains must be active for the clash"); 
    }
    ClashWinner winner{ClashWinner::None};
    DomainWinCon win_con{DomainWinCon::None};

    const bool is_equal_ref = first.refinement == second.refinement;

    if (!is_equal_ref){
        if (first.refinement > second.refinement){
            winner = ClashWinner::First;
            DomainSystem::ResetDomain(second);
        }else{
            winner = ClashWinner::Second;
            DomainSystem::ResetDomain(first);
        }
        return {&first, &second, 0.0, 0.0, winner, DomainWinCon::Refinement};
    }

    double first_damage = first.damage * second.range / first.range;
    double second_damage = second.damage * first.range / second.range;

    if (first.type == DomainType::Open && second.type == DomainType::Closed){
        first_damage *= 2.5;
    }else if (second.type == DomainType::Open && first.type == DomainType::Closed) {
        second_damage *= 2.5;
    }

    DomainSystem::HandleDamage(first, second_damage);
    DomainSystem::HandleDamage(second, first_damage);

    if (first.health <= 0.0 || second.health <= 0.0) {
        if (first.health <= 0.0 && second.health <= 0.0){
            winner = ClashWinner::Both;
        }
        else if (first.health <= 0.0){
            winner = ClashWinner::First;
        }
        else {
            winner = ClashWinner::Second;
        }
        win_con = DomainWinCon::Attrition;
    }
    return {&first, &second, first_damage, second_damage, winner, win_con};
}

SurehitStruct DomainSystem::HandleSureHit(Character &c, const Domain& dm){    
    const double damage = DomainSystem::CalculateHitDamage(dm, c);
    const auto& x = c.Damage(damage, dm.damage_type);
    if (dm.surehit_type == SurehitType::Paralyzing){
        c.State().is_stunned = true;
    }
    return{x.damage, &c, &dm};
}

void DomainSystem::HandleDamage(Domain& c, const double damage) {
    if (!c.is_active) return;
    c.health -= damage / c.durability;
    if (c.health <= 0.0){
        DomainSystem::ResetDomain(c);
    }
}

void DomainSystem::ResetDomain(Domain& domain){
    domain.is_active = false;
    domain.health = domain.max_health;
}