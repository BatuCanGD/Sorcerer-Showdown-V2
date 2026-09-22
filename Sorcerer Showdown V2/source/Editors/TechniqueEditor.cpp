#include "../../header/Editors/TechniqueEditor.hpp"
#include "../../header/Sorcery/Technique.hpp"

void TechniqueEditor::SetIdentity(Technique& t, EntityInfo id){
    t.Identity() = id;
}
void TechniqueEditor::SetBarrier(Technique &t, bool bl) {
    t.Barrier().can_use_barrier = bl;
}
void TechniqueEditor::AddAbility(Technique& t, TechAbility tb){
    t.Abilities().push_back(std::move(tb));
}