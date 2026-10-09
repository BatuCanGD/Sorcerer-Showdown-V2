#include "../../header/Systems/ResourceHandler.hpp"
#include "../../header/CharacterType/CurseUser.hpp"
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
        if (auto* crs = c->CanUseSorcery()){
            VowSystem::ApplyVows(*crs);
            ResourceHandler::TickCursedEnergy(*crs);
            ResourceHandler::TickShikigami(*crs);
            ResourceHandler::TickRCT(*crs);
            ResourceHandler::TickTraits(*crs);
            ResourceHandler::TickOutputStatus(*crs);
            ResourceHandler::TickOutput(*crs);
        }
        Log::Effects(ef.first, ef.second);
    }
}

void ResourceHandler::TickOutput(CurseUser &crs){
    crs.Output().current_output = std::max(crs.Output().current_output - crs.Output().output_cooldown_amount, 0.0);
}
void ResourceHandler::TickTraits(CurseUser &crs){
    if (crs.Traits().passive_healing){
        crs.State().health = std::min(crs.State().health + 35.0, crs.State().max_health);
    }
}
void ResourceHandler::TickCursedEnergy(CurseUser& crs){
    crs.CursedEnergy().cursed_energy = std::min(crs.CursedEnergy().cursed_energy + crs.CursedEnergy().regeneration_amount, crs.CursedEnergy().max_cursed_energy);
}
void ResourceHandler::TickShikigami(CurseUser& crs){
    for (auto& c : crs.Jujutsu().shikigami){
        ShikigamiSystem::TickShikigami(c, crs);
    }
}
void ResourceHandler::TickRCT(CurseUser& crs) {
    if (!crs.RCT().can_use_rct) return;
    const double& output = crs.RCT().rct_output;
    crs.CursedEnergy().cursed_energy = std::clamp(crs.CursedEnergy().cursed_energy - SorcerySystem::ApplySpendingMultiplier(SorcerySystem::ApplyRCTCost(output), crs), 0.0, crs.CursedEnergy().max_cursed_energy);
    crs.State().health = std::min(crs.State().health + output, crs.State().max_health);
}
void ResourceHandler::TickOutputStatus(CurseUser &crs){
    auto& op = crs.Output();
    if (op.status == CurseUserOutput::Status::Regular){
        return;
    }
    const auto burnt_out = op.status == CurseUserOutput::Status::BurntOut;
    ++op.normalize_tick;
    const std::uint8_t normalize_at = burnt_out ? 3 : 5;

    if (op.normalize_tick >= normalize_at){
        op.status = CurseUserOutput::Status::Regular;
        op.normalize_tick = 0;
        return;
    }
}