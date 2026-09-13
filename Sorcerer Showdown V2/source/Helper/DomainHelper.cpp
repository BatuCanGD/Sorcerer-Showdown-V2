#include "../../header/Helper/DomainHelper.hpp"
#include "../../header/Sorcery/Domain.hpp"
#include "../../header/CharacterType/CurseUser.hpp"

#include <optional>

std::tuple<bool, double> DomainHelper::CalculateHit(const std::optional<Domain>& caster_domain, const std::unique_ptr<Character>& character){
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
