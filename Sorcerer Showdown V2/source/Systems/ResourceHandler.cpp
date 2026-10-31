#include "../../header/Systems/ResourceHandler.hpp"
#include "../../header/Character.hpp"
#include "../../header/Systems/Mechanics/SorcerySystem.hpp"
#include "../../header/Systems/Mechanics/ShikigamiSystem.hpp"
#include "../../header/Systems/Mechanics/TechniqueSystem.hpp"
#include "../../header/Logger.hpp"
#include "../../header/Systems/Mechanics/VowSystem.hpp"
#include "../../header/Systems/Mechanics/EffectSystem.hpp"
#include "../../header/Battlefield.hpp"

#include <algorithm>

void ResourceHandler::TickAll(Battlefield& bf){
    for (auto& c : bf.battlefield){ // ordering doesnt matter
        const auto ef = EffectSystem::ApplyEffects(*c);
        ResourceHandler::TickStun(*c);
        if (c->curseUser.has_value()){
            VowSystem::ApplyVows(*c);
            ResourceHandler::TickShikigami(*c);
            ResourceHandler::TickChant(*c);
            ResourceHandler::TickRCT(*c);
            ResourceHandler::TickTraits(*c);
            ResourceHandler::TickCursedEnergy(c->curseUser);
            ResourceHandler::TickAmp(c->curseUser);
            ResourceHandler::TickTechBarrier(c->curseUser);
            ResourceHandler::TickOutputStatus(c->curseUser);
            ResourceHandler::TickOutput(c->curseUser);
        }
        Log::Effects(ef.first, ef.second);
    }
}

void ResourceHandler::TickStun(Character &c){
    auto& st = c.state;
    if (c.state.is_stunned){
        st.stun_timer++;
        if (st.stun_timer >= 3){
            st.is_stunned = false;
            st.stun_timer = 0;
        }
    }
}

void ResourceHandler::TickTechBarrier(std::optional<Sorcery::CurseUserSystem> &c){
    if (!c.has_value()) return;
    if (auto& t = c->jujutsu.technique){
        if (TechniqueSystem::HasBarrier(*t)){
            const double cost = TechniqueSystem::GetBarrierCost(*t);
            if (c->ce_system.cursed_energy > cost) {
                c->ce_system.cursed_energy -= cost;
            }else{
                t->barrier.is_active = false;
            }
        }
    }
}

void ResourceHandler::TickOutput(std::optional<Sorcery::CurseUserSystem>&crs){
    crs->output.current_output = std::max(crs->output.current_output - crs->output.output_cooldown_amount * SorcerySystem::OutputStatusMultiplier(crs->output.status), 0.0);
}
void ResourceHandler::TickTraits(Character &crs){
    if (crs.curseUser->traits.passive_healing){
        crs.state.health = std::min(crs.state.health + 35.0, crs.state.max_health);
    }
}
void ResourceHandler::TickCursedEnergy(std::optional<Sorcery::CurseUserSystem>& crs){
    crs->ce_system.cursed_energy = std::min(crs->ce_system.cursed_energy + crs->ce_system.regeneration_amount, crs->ce_system.max_cursed_energy);
}
void ResourceHandler::TickAmp(std::optional<Sorcery::CurseUserSystem>&crs){
    if (!crs.has_value()){
        return;
    }
    auto& amp = crs->amplification;
    if (amp.can_use_amp && amp.is_active){
        auto& c = crs->ce_system;
        const double cost = c.max_cursed_energy * 0.01;
        if (c.cursed_energy < cost){
            amp.is_active = false;
        }else{
            c.cursed_energy -= cost;
        }
    }
}
void ResourceHandler::TickShikigami(Character& crs){
    for (auto& c : crs.curseUser->jujutsu.shikigami){
        ShikigamiSystem::TickShikigami(c, crs);
    }
}
void ResourceHandler::TickChant(Character &crs){
    auto& cr = crs.curseUser;
    if (!cr->jujutsu.technique) return;
    for (auto& c : cr->jujutsu.technique->abilities) {
        if (c.chant_amount > 0) c.chant_amount = std::max(c.chant_amount - 2, 0);
    }
}
void ResourceHandler::TickRCT(Character& crs) {
    auto& cr = crs.curseUser;
    if (!cr->rct_system.can_use_rct) return;
    double& output = cr->rct_system.rct_output;
    const double cost = SorcerySystem::ApplySpendingMultiplier(SorcerySystem::ApplyRCTCost(output), crs);
    if (cost > cr->ce_system.cursed_energy){
        return;
    }
    cr->ce_system.cursed_energy -= cost;
    crs.state.health = std::min(crs.state.health + output, crs.state.max_health);
}
void ResourceHandler::TickOutputStatus(std::optional<Sorcery::CurseUserSystem>& crs){
    if (!crs.has_value()){
        return;
    }
    auto& op = crs->output;
    if (op.status == Sorcery::OutputSystem::Status::Regular){
        return;
    }
    const auto burnt_out = op.status == Sorcery::OutputSystem::Status::BurntOut;
    ++op.normalize_tick;
    const std::uint8_t normalize_at = burnt_out ? 3 : 5;

    if (op.normalize_tick >= normalize_at){
        op.status = Sorcery::OutputSystem::Status::Regular;
        op.normalize_tick = 0;
        return;
    }
}