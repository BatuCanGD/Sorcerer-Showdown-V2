#include "../../header/Systems/BattlefieldSystem.hpp"
#include "../../header/Systems/System/DomainSystem.hpp"
#include "../../header/Systems/System/SorcerySystem.hpp"
#include "../../header/Systems/System/NeutralizerSystem.hpp"
#include "../../header/CharacterType/CurseUser.hpp"
#include "../../header/Logger.hpp"
#include "../../header/Battlefield.hpp"

#include <vector>
#include <print>


void BattlefieldSystem::HandleDomainInteraction(Battlefield &bf){
    std::vector<CurseUser*> domain_users;
    for (const auto& c : bf.battlefield){
        if (auto* cr = c->CanUseSorcery()){
            double& ce = cr->CursedEnergy().cursed_energy;
            if (auto& d = cr->Jujutsu().domain){
                if (d->is_active){
                    if (ce < d->cost){
                        DomainSystem::ResetDomain(cr->Jujutsu().domain);
                    }else {
                        ce -= SorcerySystem::ApplySpendingMultiplier(d->cost, *cr);
                        domain_users.push_back(cr);
                    }
                }
            }
            if (auto& n = cr->Jujutsu().neutralizer){
                if (n->is_active){
                    if (ce < n->cost){
                        NeutralizerSystem::ResetNeutralizer(*n);
                    }else {
                        ce -= SorcerySystem::ApplySpendingMultiplier(n->cost, *cr);
                    }
                }
            }
        }
    }
    switch (domain_users.size()) {
        case 0: {
            std::println("No domains are active this turn");
            break;
        }
        case 1: {
            [[maybe_unused]] const auto l = CombatSystem::ResolveDomain(*domain_users[0], bf);
            break;
        }
        case 2: {
            [[maybe_unused]] const auto l = DomainSystem::ClashDomains(domain_users[0]->Jujutsu().domain, domain_users[1]->Jujutsu().domain);
            break;
        }
        default: {
            for (auto* c : domain_users) {
                DomainSystem::ResetDomain(c->Jujutsu().domain);
            }
            break;
        }
    }
}

void BattlefieldSystem::HandleDeadPeople(Battlefield &bf){
    Log::Death(bf);
    std::erase_if(bf.battlefield, [](const auto& s) { 
        return s->State().health <= 0.0;
    });
}
void BattlefieldSystem::HandleSpawns(Battlefield &bf) {
    for (auto& c : bf.spawn_next){
        bf.battlefield.push_back(std::move(c));
    }
}