#include "../../header/Editors/TechniqueEditor.hpp"
#include "../../header/Sorcery/Technique.hpp"

void TechniqueEditor::SetIdentity(Technique& t, EntityInfo id){
    t.identity = id;
}
void TechniqueEditor::SetBarrier(Technique &t, bool bl) {
    t.barrier.can_use_barrier = bl;
}
void TechniqueEditor::AddAbility(Technique& t, TechAbility& tb){
    t.abilities.push_back(std::move(tb));
}