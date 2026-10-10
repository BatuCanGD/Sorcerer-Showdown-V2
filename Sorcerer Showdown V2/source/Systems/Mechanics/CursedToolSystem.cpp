#include "../../../header/Systems/Mechanics/CursedToolSystem.hpp"
#include "../../../header/General/CursedTool.hpp"

bool CursedToolSystem::DoesBypassTech(const CursedTool& c) {
    return c.damage_type == globalums::DamageType::BypassTech || c.damage_type == globalums::DamageType::BypassAll;
}
bool CursedToolSystem::DoesBypassRein(const CursedTool& c) {
    return c.damage_type == globalums::DamageType::BypassRein || c.damage_type == globalums::DamageType::BypassAll;
}