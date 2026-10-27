#include "../header/DamageType.hpp"

bool DamageCheck::BypassesTech(DamageType dt){
    return dt == DamageType::BypassTech || dt == DamageType::BypassAll;
}
bool DamageCheck::BypassesReinforcement(DamageType dt){
    return dt == DamageType::BypassRein || dt == DamageType::BypassAll;
}
