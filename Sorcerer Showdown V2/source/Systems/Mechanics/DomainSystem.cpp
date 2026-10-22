#include "../../../header/Systems/Mechanics/DomainSystem.hpp"
#include "../../../header/Systems/Mechanics/CombatSystem.hpp"
#include "../../../header/Systems/Mechanics/SorcerySystem.hpp"
#include "../../../header/CharacterType/CurseUser.hpp"
#include "../../../header/Sorcery/Domain.hpp"



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
        if (const auto& nl = cc->Jujutsu().neutralizer; nl && nl->is_active){
            return nl->neutralizer_type == NeutralizerType::ReducedDamage && domain.surehit_type != SurehitType::Basic;
        }
    }
    return true;
}

DomainClashStruct DomainSystem::ClashDomains(CurseUser& f, CurseUser& s) {
    auto& first  = f.Jujutsu().domain;
    auto& second = s.Jujutsu().domain;
    if (!(first->is_active && second->is_active)) {
        throw std::invalid_argument("Both domains must be active for the clash"); 
    }
    
    if (first->refinement != second->refinement){
        ClashWinner winner{};
        if (first->refinement > second->refinement){
            winner = ClashWinner::First;
            DomainSystem::ResetDomain(*second, &s);
        }else{
            winner = ClashWinner::Second;
            DomainSystem::ResetDomain(*first, &f);
        }
        return {&*first, &*second, winner, DomainWinCon::Refinement};
    }

    const bool d1_destroyed = HandleDamage(*first, GetDomainDamage(*first, *second), &f);
    const bool d2_destroyed = HandleDamage(*second, GetDomainDamage(*second, *first), &s);

    if (d1_destroyed || d2_destroyed) {
        ClashWinner winner{};
        if (d1_destroyed && d2_destroyed){
            winner = ClashWinner::Both;
        }else if (d2_destroyed){
            winner = ClashWinner::First;
        }else{
            winner = ClashWinner::Second;
        }
        return {&*first, &*second, winner, DomainWinCon::Attrition};
    }
    return {&*first, &*second, ClashWinner::None, DomainWinCon::None};
}

double DomainSystem::GetDomainDamage(const Domain &d1, const Domain &d2) {
    double damage = d1.damage * d2.range / d1.range;
    if (d1.type == DomainType::Open && d2.type == DomainType::Closed){
        damage *= 2.5;
    }
    damage /= d2.durability;
    return damage;
}

void DomainSystem::HandleLifetime(CurseUser &c, Domain &d) {
    if (!d.is_active) {
        return;
    }
    const double cost = SorcerySystem::ApplySpendingMultiplier(d.cost, c);
    if (c.CursedEnergy().cursed_energy < cost) {
        ResetDomain(d);
    }else{
        c.CursedEnergy().cursed_energy -= cost;
    }
}

SurehitStruct DomainSystem::HandleSureHit(Character &c, const Domain& dm){    
    const double damage = CalculateHitDamage(dm, c);
    const auto& x = c.Damage(damage, dm.damage_type);
    if (dm.surehit_type == SurehitType::Paralyzing)c.State().is_stunned = true;
    return{x.damage, &c, &dm};
}

bool DomainSystem::HandleDamage(Domain& c, const double damage, CurseUser* crs) {
    if (!c.is_active) {
        throw std::runtime_error("Inactive domains cannot be damaged");
    }
    c.health -= damage;
    if (c.health <= 0.0){
        ResetDomain(c, crs);
        return true;
    }
    return false;
}

void DomainSystem::ResetDomain(Domain& domain, CurseUser* crs){
    domain.is_active = false;
    domain.health = domain.max_health;
    if (crs){
        SorcerySystem::ApplyBurnOut(*crs);
    }
}