#include "../header/CharacterType/CurseUser.hpp"
#include "../header/Creator.hpp"
#include "../header/Battlefield.hpp"
#include "../header/Systems/Mechanics/DomainSystem.hpp"
#include "../header/Systems/BattlefieldSystem.hpp"
#include "../header/Systems/Mechanics/CombatSystem.hpp"
#include "../header/Systems/Mechanics/TechniqueSystem.hpp"
#include "../header/Sorcery/Technique.hpp"
#include "../header/Sorcery/Domain.hpp"

#include <gtest/gtest.h>
#include <optional>

TEST(DomainSystemTest, AttritionWinCheck){
    CurseUser crs1 = CurseUser{};
    CurseUser crs2 = CurseUser{};
    crs1.Jujutsu().domain = Domain{.damage = 50.0,.is_active = true};
    crs2.Jujutsu().domain = Domain{.health = 1.0, .is_active = true};
    const auto winner = DomainSystem::ClashDomains(crs1, crs2);
    ASSERT_EQ(winner.win_condition, DomainWinCon::Attrition);
}

TEST(DomainSystemTest, RefinementWinCheck){
    CurseUser crs1 = CurseUser{};
    CurseUser crs2 = CurseUser{};
    crs1.Jujutsu().domain = Domain{.is_active = true, .refinement = Refinement::Absolue};
    crs2.Jujutsu().domain = Domain{.is_active = true, .refinement = Refinement::Brittle};
    const auto winner = DomainSystem::ClashDomains(crs1, crs2);
    ASSERT_EQ(winner.winner, ClashWinner::First);
}

TEST(BattlefieldSystemTest, SingleDomainSurehitCheck){
    Battlefield bf;
    bf.battlefield.push_back(std::make_unique<CurseUser>(Create::Gojo()));
    bf.battlefield.push_back(std::make_unique<CurseUser>(Create::Gojo()));
    static_cast<CurseUser*>(bf.battlefield[0].get())->Jujutsu().domain = Domain{.damage = 5000.0, .is_active = true, .damage_type = globalums::DamageType::BypassAll};
    BattlefieldSystem::HandleDomainInteraction(bf);
    ASSERT_TRUE(bf.battlefield[1]->State().health < 0.0);
}

TEST(TechniqueSystemTest, ProperDamageCheck) {
    Technique t = Technique{};
    t.Abilities().push_back({.id{"Lime Green"}, .damage = 50.0, .cost = 0.0, .output = 0.0});
    CurseUser crs = CurseUser{};
    Character chr = Character{};
    chr.State().max_health = 50.0;
    chr.State().health = chr.State().max_health;
    chr.State().durability = 0.0;
    CombatSystem::ResolveTechnique(crs, t.Abilities()[0], chr);
    EXPECT_DOUBLE_EQ(chr.State().health, 0.0);
}
TEST(TechniqueSystemTest, DamageNegationCheck){
    Technique t = Technique{};
    t.Abilities().push_back({.id{"Lime Green"}, .damage = 50.0, .cost = 0.0, .output = 0.0});
    CurseUser crs = CurseUser{};
    Character chr = Character{};
    chr.State().max_health = 50.0;
    chr.State().health = chr.State().max_health;
    chr.State().durability = 100.0;
    CombatSystem::ResolveTechnique(crs, t.Abilities()[0], chr);
    EXPECT_NE(chr.State().health, 0.0);
}
TEST(TechniqueSystemTest, BarrierCheck){
    Character chr = Character{};
    CurseUser crs = CurseUser{};
    chr.State().strength = 100000.0;
    crs.Jujutsu().technique = Create::Limitless();
    crs.State().durability = 0.0;
    CombatSystem::ResolveAttacking(chr, crs);
    ASSERT_DOUBLE_EQ(crs.State().health, crs.State().max_health);
}

TEST(CombatSystemTest, BlackFlashTest){
    CurseUser crs = CurseUser{};
    Character chr = Character{};
    crs.CursedEnergy().bf_chance = 100; // 2.5 multiplier with blackflash
    crs.State().strength = 50.0; // should be 125.0 damage
    chr.State().max_health = 250.0;
    chr.State().health = chr.State().max_health;
    chr.State().durability = 0.0;
    const auto r = CombatSystem::ResolveAttacking(crs, chr);
    ASSERT_TRUE(r.is_blackflash);
    ASSERT_DOUBLE_EQ(chr.State().health, 125.0);
}
TEST(CombatSystemTest, BasicDamageTest){
    Character c1 = Character{};// should be both auto initialized with all doubles set to 1.0
    Character c2 = Character{}; 
    c2.State().durability = 0.0;
    CombatSystem::ResolveAttacking(c1, c2);
    ASSERT_DOUBLE_EQ(c2.State().health, 0.0);
}