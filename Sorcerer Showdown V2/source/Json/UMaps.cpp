#include "../../header/Json.hpp"
#include "../../header/Creator.hpp"

UMap<CursedTool> UMaps::GetCTMap() {
    return {
        {"Split Soul Katana", Create::Tool::SplitSoulKatana()},
        {"Inverted Spear of Heaven", Create::Tool::InvertedSpearOfHeaven()},
        {"Playful Cloud", Create::Tool::PlayfulCloud()}
    };
}

UMap<Shikigami> UMaps::GetSKMap() {
    return {
        {"Mahoraga", Create::Shk::Mahoraga()},
        {"Agito", Create::Shk::Agito()},
        {"Ember Insects", Create::Shk::EmberInsects()}
    };
}

UMap<ResourceUsage> UMaps::GetRUMap() {
    return {
        {"All Out", ResourceUsage::AllOut},
        {"Conservative", ResourceUsage::Conservative}
    };
}

UMap<FightingStyle> UMaps::GetFSMap(){
    return {
        {"Aggressive", FightingStyle::Aggressive},
        {"Defensive", FightingStyle::Defensive},
    };
}

UMap<Technique> UMaps::GetTCMap() {
    return {
        {"Limitless", Create::Tech::Limitless()},
        {"Shrine", Create::Tech::Shrine()},
        {"Idle Transfiguration", Create::Tech::IdleTransfiguration()},
        {"Disaster Plants", Create::Tech::DisasterPlants()},
        {"Disaster Flames", Create::Tech::DisasterFlames()}
    };
}

UMap<Domain> UMaps::GetDMMap() {
    return {
        {"Unlimited Void", Create::Dmn::UnlimitedVoid()},
        {"Malevolent Shrine", Create::Dmn::MalevolentShrine()},
        {"Self Embodiment Of Perfection", Create::Dmn::SelfEmbodimentOfPerfection()},
        {"Coffin Of The Iron Mountain", Create::Dmn::CoffinOfTheIronMountain()},
        {"Shining Sea Of Growing Branches", Create::Dmn::ShiningSeaOfGrowingBranches()}
    };
}

UMap<Neutralizer> UMaps::GetNLMap() {
    return {
        {"Hollow Wicker Basket", Create::Nlr::HollowWickerBasket()},
        {"Simple Domain", Create::Nlr::SimpleDomain()},
        {"Falling Blossom Emotion", Create::Nlr::FallingBlossomEmotion()}
    };
}

UMap<StatusEffect> UMaps::GetSTMap() {
    return {
        {"Bleed", Create::Effect::Bleed()}
    };
}

UMap<EffectForType> UMaps::GetEFTMap() {
    return {
        {"Health", EffectForType::Health},
        {"Durability", EffectForType::Durability},
        {"Strength", EffectForType::Strength},
        {"Cursed Energy", EffectForType::CursedEnergy},
        {"Max Output", EffectForType::MaxOutput}
    };
}

UMap<DamageType> UMaps::GetDTMap() {
    return {
        {"Bypass All", DamageType::BypassAll},
        {"Bypass Reinforcement", DamageType::BypassRein},
        {"Bypass Techniques", DamageType::BypassTech},
    };
}

UMap<SupportType> UMaps::GetSPTMap() {
    return {
        {"Strength", SupportType::Offense},
        {"Durability", SupportType::Defense},
        {"Cursed Energy Regeneration", SupportType::CursedEnergyRegen},
        {"Output", SupportType::Output}
    };
}

UMap<ActiveAbility> UMaps::GetAAMap() {
    return {
        {"Technique Bypassing", ActiveAbility::TechniqueBypass}
    };
}

UMap<Refinement> UMaps::GetRFMap() {
    return {
        {"Absolute", Refinement::Absolue},
        {"Overwhelming", Refinement::Overwhelming},
        {"Adept", Refinement::Adept},
        {"Brittle", Refinement::Brittle},
        {"Weak", Refinement::Weak}
    };
}

UMap<Sorcery::CursedEnergySystem::Efficiency> UMaps::GetEFMap() {
    return {
        {"Peak", Sorcery::CursedEnergySystem::Efficiency::Peak},
        {"Extreme", Sorcery::CursedEnergySystem::Efficiency::Extreme},
        {"Expert", Sorcery::CursedEnergySystem::Efficiency::Expert},
        {"Precise", Sorcery::CursedEnergySystem::Efficiency::Precise},
        {"Stable", Sorcery::CursedEnergySystem::Efficiency::Stable},
        {"Unstable", Sorcery::CursedEnergySystem::Efficiency::Unstable},
        {"Rough", Sorcery::CursedEnergySystem::Efficiency::Rough},
        {"Wasteful", Sorcery::CursedEnergySystem::Efficiency::Wasteful}
    };
}

UMap<TargetingType> UMaps::GetTTMap() {
    return {
        {"Highest HP", TargetingType::HighestHP},
        {"Lowest HP", TargetingType::LowestHP}
    };
}

UMap<SurehitType> UMaps::GetSHMap() {
    return {
        {"Basic", SurehitType::Basic},
        {"Paralyzing", SurehitType::Paralyzing},
    };
}