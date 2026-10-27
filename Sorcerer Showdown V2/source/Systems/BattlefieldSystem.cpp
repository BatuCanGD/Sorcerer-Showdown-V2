#include "../../header/Systems/BattlefieldSystem.hpp"
#include "../../header/Systems/Mechanics/DomainSystem.hpp"
#include "../../header/Systems/Mechanics/NeutralizerSystem.hpp"
#include "../../header/Systems/Mechanics/ShikigamiSystem.hpp"
#include "../../header/Character.hpp"
#include "../../header/Logger.hpp"
#include "../../header/Battlefield.hpp"
#include <vector>


void BattlefieldSystem::HandleDomainInteraction(Battlefield &bf){
    std::vector<Character*> domain_users;
    for (auto& c : bf.battlefield){
        if (auto cr = c->curseUser){
            if (auto& d = c->curseUser->jujutsu.domain){
                DomainSystem::HandleLifetime(*c, *d);
                if (d->is_active){
                    domain_users.push_back(&*c);
                }
            }
            if (auto& n = cr->jujutsu.neutralizer){
                NeutralizerSystem::HandleLifeTime(*c, *n);
            }
        }
    }
    switch (domain_users.size()) {
        case 0: {
            Log::Line("No domains are active this turn");
            break;
        }
        case 1: {
            CombatSystem::ResolveDomain(*domain_users[0], bf);
            break;
        }
        case 2: {
            const auto l = DomainSystem::ClashDomains(*domain_users[0], *domain_users[1]);
            Log::Clash(l);
            break;
        }
        default: {
            for (auto* c : domain_users) {
                DomainSystem::ResetDomain(*c->curseUser->jujutsu.domain, c);
            }
            break;
        }
    }
}

void BattlefieldSystem::HandleDeadShikigami(Battlefield &bf){
    for (auto& c : bf.battlefield){
        if (auto& cr = c->curseUser){
            for (auto& sh : cr->jujutsu.shikigami){
                if (sh.stats.health <= 0.0){
                   ShikigamiSystem::HandleShadow(*c, sh.support_type, sh.rollback);
                }
            }
            std::erase_if(cr->jujutsu.shikigami, [](const auto& s) { 
                return s.stats.health <= 0.0;
            });
        }
    }
}
void BattlefieldSystem::HandleDeadPeople(Battlefield &bf){
    Log::Death(bf);
    std::erase_if(bf.battlefield, [](const auto& s) { 
        return s->state.health <= 0.0;
    });
}

void BattlefieldSystem::HandleSpawns(Battlefield &bf) {
    for (auto& c : bf.spawn_next){
        bf.battlefield.push_back(std::move(c));
    }
    bf.spawn_next.clear();
}
bool BattlefieldSystem::CanContinueFight(Battlefield &bf){
    std::uint8_t i{};
    for (const auto& c : bf.battlefield){
        if (c->state.health > 0.0 && ++i > 1) {
            return true;
        }
    }
    return false;
}