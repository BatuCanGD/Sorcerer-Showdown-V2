#include "../../header/Systems/ResourceHandler.hpp"
#include "../../header/Character.hpp"
#include "../../header/Systems/Mechanics/SorcerySystem.hpp"
#include "../../header/Systems/Mechanics/ShikigamiSystem.hpp"
#include "../../header/Logger.hpp"
#include "../../header/Systems/Mechanics/VowSystem.hpp"
#include "../../header/Systems/Mechanics/EffectSystem.hpp"
#include "../../header/Battlefield.hpp"

#include <algorithm>

void ResourceHandler::TickAll(Battlefield& bf){
    for (auto& c : bf.battlefield){
        const auto ef = EffectSystem::ApplyEffects(*c);
        if (c->curseUser.has_value()){
            VowSystem::ApplyVows(*c);
            ResourceHandler::TickCursedEnergy(*c);
            ResourceHandler::TickShikigami(*c);
            ResourceHandler::TickChant(*c);
            ResourceHandler::TickAmp(*c);
            ResourceHandler::TickRCT(*c);
            ResourceHandler::TickTraits(*c);
            ResourceHandler::TickOutputStatus(*c);
            ResourceHandler::TickOutput(*c);
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

void ResourceHandler::TickOutput(Character &crs){
    auto& cr = crs.curseUser;
    cr->output.current_output = std::max(cr->output.current_output - cr->output.output_cooldown_amount * SorcerySystem::OutputStatusMultiplier(cr->output.status), 0.0);
}
void ResourceHandler::TickTraits(Character &crs){
    if (crs.curseUser->traits.passive_healing){
        crs.state.health = std::min(crs.state.health + 35.0, crs.state.max_health);
    }
}
void ResourceHandler::TickCursedEnergy(Character& crs){
    auto& cr = crs.curseUser;
    cr->ce_system.cursed_energy = std::min(cr->ce_system.cursed_energy + cr->ce_system.regeneration_amount, cr->ce_system.max_cursed_energy);
}
void ResourceHandler::TickAmp(Character &crs){
    auto& cr = crs.curseUser;
    auto& amp = cr->amplification;
    if (amp.can_use_amp && amp.is_active){
        auto& c = cr->ce_system;
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
void ResourceHandler::TickOutputStatus(Character &crs){
    auto& op = crs.curseUser->output;
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