#include "../../../header/Systems/System/DomainSystem.hpp"
#include "../../../header/Sorcery/Domain.hpp"
#include "../../../header/CharacterType/CurseUser.hpp"

#include <optional>

std::pair<bool, double> DomainSystem::CalculateHit(const std::optional<Domain>& caster_domain, const std::unique_ptr<Character>& character){
    bool can_hit{true}; 
    double damage{caster_domain->damage};

    if (const auto* c = character->CanUseSorcery()) { 
        if (const auto& nl = c->Jujutsu().domain_neutralizer){
            if (nl->is_active){
                if (nl->neutralizer_type == NeutralizerType::ReducedDamage){
                    if (caster_domain->surehit_type == SurehitType::Basic){
                        can_hit = false;
                    }else{
                        damage /= 2.5;
                    }
                }else{
                    can_hit = false;
                }
            }
        }
    }
    return {can_hit, damage};
}

std::pair<ClashWinner, DomainWinCon> DomainSystem::ClashDomains(std::optional<Domain> &first, std::optional<Domain> &second) {
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

    const bool is_equal_range = first->range == second->range;

    if (!is_equal_range){
        if (first->range > second->range * 2){
            DomainSystem::ResetDomain(second);
            return {ClashWinner::First, DomainWinCon::Overwhelmed};
        }else if (second->range > first->range * 2){
            DomainSystem::ResetDomain(first);
            return {ClashWinner::Second, DomainWinCon::Overwhelmed};
        }
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

void DomainSystem::HandleSureHit(Character &c, const double damage, const bool does_paralyze){
    c.Health(OpType::Expend, damage);
    if (does_paralyze){
        c.State().is_stunned = true;
    }
}

void DomainSystem::ResetDomain(std::optional<Domain>& domain){
    domain->is_active = false;
    domain->health = domain->max_health;
}