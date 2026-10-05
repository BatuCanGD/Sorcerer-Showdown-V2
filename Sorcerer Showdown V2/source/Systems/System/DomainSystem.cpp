#include "../../../header/Systems/System/DomainSystem.hpp"
#include "../../../header/Sorcery/Domain.hpp"
#include "../../../header/CharacterType/CurseUser.hpp"
#include "../../../header/Systems/System/CombatSystem.hpp"

#include <optional>
#include <stdexcept>


double CalculateHitDamage(const std::optional<Domain>& domain, const std::unique_ptr<Character>& c) {
    double damage{domain->damage};
    if (const auto* cc = c->CanUseSorcery()) { 
        if (const auto& nl = cc->Jujutsu().neutralizer){
            if (nl->is_active && nl->neutralizer_type == NeutralizerType::ReducedDamage){
                damage /= 2.5;
            }
        }
    }
    return damage;
}
bool CalculateActualHit(const std::optional<Domain>& domain, const std::unique_ptr<Character>& c) {
    if (const auto* cc = c->CanUseSorcery()) { 
        if (const auto& nl = cc->Jujutsu().neutralizer){
            if (nl->is_active){
                return nl->neutralizer_type == NeutralizerType::ReducedDamage && domain->surehit_type != SurehitType::Basic;
            }
        }
    }
    return true;
}

DomainClashStruct DomainSystem::ClashDomains(std::optional<Domain> &first, std::optional<Domain> &second) {
    if (!(first->is_active && second->is_active)) {
        throw std::invalid_argument("Both domains must be active for the clash"); 
    }
    ClashWinner winner{ClashWinner::None};
    DomainWinCon win_con{DomainWinCon::None};

    const bool is_equal_ref = first->refinement == second->refinement;

    if (!is_equal_ref){
        if (first->refinement > second->refinement){
            winner = ClashWinner::First;
            DomainSystem::ResetDomain(second);
        }else{
            winner = ClashWinner::Second;
            DomainSystem::ResetDomain(first);
        }
        return {winner, DomainWinCon::Refinement};
    }

    double first_damage = first->damage * second->range / first->range;
    double second_damage = second->damage * first->range / second->range;

    if (first->type == DomainType::Open && second->type == DomainType::Closed){
        first_damage *= 2.5;
    }else if (second->type == DomainType::Open && first->type == DomainType::Closed) {
        second_damage *= 2.5;
    }

    first->health -= second_damage / first->durability;
    second->health -= first_damage / second->durability;

    if (first->health <= 0.0 || second->health <= 0.0) {
        if (first->health <= 0.0 && second->health <= 0.0){
            winner = ClashWinner::Both;
        }
        else if (first->health <= 0.0){
            winner = ClashWinner::First;
        }
        else {
            winner = ClashWinner::Second;
        }
        win_con = DomainWinCon::Attrition;
    }
    return {winner, win_con};
}

SurehitStruct DomainSystem::HandleSureHit(Character &c, const std::optional<Domain>& dm){    
    const auto& x = c.Damage(dm->damage, dm->damage_type);
    if (dm->surehit_type == SurehitType::Paralyzing){
        c.State().is_stunned = true;
    }
    return{x.damage, &c, &*dm};
}

void DomainSystem::ResetDomain(std::optional<Domain>& domain){
    domain->is_active = false;
    domain->health = domain->max_health;
}