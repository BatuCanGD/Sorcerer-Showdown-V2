#include "../../header/Sorcery/Technique.hpp"

#include <stdexcept>

const EntityInfo& Technique::Identity() const noexcept{
    return identity;
}
EntityInfo& Technique::Identity() noexcept{
    return identity;
}
const TechBarrier& Technique::Barrier() const noexcept {
    return barrier;
}
TechBarrier& Technique::Barrier() noexcept{
    return barrier;
}
const std::vector<TechAbility>& Technique::Abilities() const noexcept {
    return abilities;
}
std::vector<TechAbility>& Technique::Abilities() noexcept{
    return abilities;
}

void Technique::Barrier(bool set_active) {
    barrier.is_active = set_active;
}
bool Technique::HasBarrier() const noexcept {
    return barrier.can_use_barrier && barrier.is_active;
}

const std::string Technique::Name() const noexcept {
    return identity.color + identity.name + (identity.color.empty() ? "" : "\x1b[0m");
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