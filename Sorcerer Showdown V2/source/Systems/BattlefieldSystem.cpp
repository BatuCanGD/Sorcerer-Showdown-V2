#include "../../header/Systems/BattlefieldSystem.hpp"
#include "../../header/Systems/System/DomainSystem.hpp"
#include "../../header/CharacterType/CurseUser.hpp"
#include "../../header/Logger.hpp"
#include "../../header/Battlefield.hpp"

#include <vector>
#include <print>


void BattlefieldSystem::HandleDomainInteraction(Battlefield &bf){
    std::vector<CurseUser*> cbv;
    for (const auto& c : bf.battlefield){
        if (auto* cr = c->CanUseSorcery()){
            const double& ce = cr->CursedEnergy().cursed_energy;
            if (cr->Jujutsu().domain && cr->Jujutsu().domain->is_active){
                if (ce < cr->Jujutsu().domain->cost){
                    DomainSystem::ResetDomain(cr->Jujutsu().domain);
                    continue;
                }
                cr->CursedEnergy().cursed_energy -= cr->Jujutsu().domain->cost;
                cbv.push_back(cr);
            }
            if (cr->Jujutsu().neutralizer && cr->Jujutsu().neutralizer->is_active){
                cr->CursedEnergy().cursed_energy -= cr->Jujutsu().neutralizer->cost;
            }
        }
    }
    if (cbv.size() >= 3){
        for (auto& c : cbv){
            DomainSystem::ResetDomain(c->Jujutsu().domain);
        }
    }else if (cbv.size() == 2){
        DomainSystem::ClashDomains(cbv[0]->Jujutsu().domain, cbv[1]->Jujutsu().domain);
    }else if (cbv.size() == 1){
        CombatSystem::ResolveDomain(*cbv[0], bf);
    }else {
        std::println("No domains are active this turn");
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