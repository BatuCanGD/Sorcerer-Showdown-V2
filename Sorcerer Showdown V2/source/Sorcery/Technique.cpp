#include "../../header/Sorcery/Technique.hpp"

#include <print>
#include <cstdint>
#include <stdexcept>

const EntityInfo& Technique::Identity() const noexcept{
    return identity;
}
const TechBarrier& Technique::Barrier() const noexcept {
    return barrier;
}
void Technique::Barrier(bool set_active) {
    barrier.is_active = set_active;
}
bool Technique::HasBarrier() const noexcept {
    return barrier.can_use_barrier && barrier.is_active;
}

void Technique::PrintName() const {
    std::println("{}{}{}", identity.color, identity.name, identity.color.empty() ? "" : "\x1b[0m");
}
void Technique::PrintAbilities() const {
    std::uint8_t z = 0;
    for ([[maybe_unused]] const auto& [id, damage, cost, output, at_type] : abilities){
        std::println("{}:[{}{}{}] {:.1f} damage |{:.1f} cursed energy cost |{:.1f} output cost", 
            ++z, id.color, id.name, id.color.empty() ? "" : "\x1b[0m", damage, cost, output);
    }
}

const TechAbility& Technique::GetAbility(size_t idx) const {
    if (idx >= abilities.size()){
        throw std::invalid_argument("Invalid Value");
    }
    return abilities[idx];
}
size_t Technique::GetAbility() const{
    return abilities.size();
}