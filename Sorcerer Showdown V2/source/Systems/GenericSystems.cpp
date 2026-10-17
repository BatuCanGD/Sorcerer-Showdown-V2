#include "../../header/Systems/GenericSystems.hpp"
#include "../../header/CharacterType/Character.hpp"


bool GenericSystems::BypassesTech(globalums::DamageType dt){
    return dt == globalums::DamageType::BypassTech || dt == globalums::DamageType::BypassAll;
}
bool GenericSystems::BypassesReinforcement(globalums::DamageType dt){
    return dt == globalums::DamageType::BypassRein || dt == globalums::DamageType::BypassAll;
}

bool GenericSystems::HPMoreThanMax(Character &c, double p) {
    return c.State().health >= c.State().max_health * p;
}