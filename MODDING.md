# Modding Guide

Welcome to MODDING.md for Sorcerer Showdown V2. This is a lengthy guide on how the project is organized and how to modify the game yourself, either by writing C++ **factories** or by dropping **JSON files** next to the game to add custom characters, techniques, domains, shikigami, cursed tools and status effects.

**Contents**

1. [Project Structure](#1-project-structure)
2. [Adding Characters](#2-adding-characters)
3. [JSON Reference](#3-json-reference)
4. [Adding Other Content in C++](#4-adding-other-content-in-c)
5. [Testing Your Mod](#5-testing-your-mod)

---

# 1. Project Structure

All code lives in `Sorcerer Showdown V2/`. Headers are in `header/`; sources are in `source/` and mirror the header layout, plus two source-only folders (`Factories/` and `Json/`) and `main/`.

## The Character Model

The game is built around composition. A `Character` is a plain struct, and everything a curse user can do is stored in an optional component. A character with no `curseUser` (Toji, Maki) is a plain fighter, and every sorcery-related system checks for the component before using it.

### Character

| Member | Type | Defined in | Description |
|--------|------|------------|-------------|
| `identity` | `EntityInfo` | `Info.hpp` | Name, ANSI color code and description |
| `state` | `Base::State` | `BaseStruct.hpp` | Health, max health, durability, strength, stun and invulnerability flags, active status effects |
| `brain` | `Base::DecisionMaker` | `BaseStruct.hpp` | AI preferences (targeting, fighting style, resource usage) |
| `equipment` | `Base::Equipment` | `BaseStruct.hpp` | Held tool, stored tool, inventory and inventory access |
| `curseUser` | `std::optional<Sorcery::CurseUserSystem>` | `SorceryStruct.hpp` | Present only for curse users |

### CurseUserSystem

| Member | Type | Description |
|--------|------|-------------|
| `jujutsu` | `JujutsuSystem` | Technique, domain, neutralizer, shikigami and binding vows |
| `ce_system` | `CursedEnergySystem` | Cursed energy, max, regeneration, reinforcement, Black Flash chance and efficiency |
| `output` | `OutputSystem` | Maximum output potential, current output, cooldown and output status (Regular, BurntOut, Boosted, Max) |
| `amplification` | `AmplificationSystem` | Domain Amplification availability and state |
| `rct_system` | `RCTSystem` | Reverse Cursed Technique availability and output |
| `traits` | `SorceryTraits` | Six Eyes and Passive Healing |

## Root Headers (`header/`)

### Core Data

| File | Description |
|------|-------------|
| `Character.hpp` | The `Character` struct. |
| `BaseStruct.hpp` | `Base::State`, `Base::Equipment` and `Base::DecisionMaker`. |
| `SorceryStruct.hpp` | Every `Sorcery::` component that makes up `CurseUserSystem`. |
| `Battlefield.hpp` | The `Battlefield` struct: `battlefield` holds the fighters, and `spawn_next` holds characters that are moved onto the battlefield at the end of the round. |
| `Info.hpp` | `EntityInfo` (name, color, description) and the `GetInfo` helpers. |
| `DamageType.hpp` | The `DamageType` enum (`Normal`, `BypassTech`, `BypassRein`, `BypassAll`) and the `DamageCheck` helpers. |

### Game Flow, AI and Output

| File | Description |
|------|-------------|
| `Game.hpp` | Declares `rungame()` and `endgame()`. The game loop itself (setup, rounds, end-of-round upkeep, win check) is in `source/Game.cpp`. |
| `AI.hpp` | The `AI` namespace: target selection, fighting and resource-management decisions, and the `Fight` entry point. |
| `AIEnums.hpp` | The AI enums: `TargetingType`, `TargetedTypes`, `FightingStyle`, `ResourceUsage`. |
| `Logger.hpp` | The `Log` namespace: `Message`, `Line` and `Error`, plus the presentation helpers for attacks, techniques, domains and deaths. |

### Content Creation and Loading

| File | Description |
|------|-------------|
| `Creator.hpp` | Declares the built-in factories in the `Create::` namespace: `Char`, `Tech`, `Dmn`, `Nlr`, `Shk`, `Tool` and `Effect`. |
| `Json.hpp` | Everything JSON: `JsonModify` (build an entity from JSON), `JsonGet` (look entities and enums up by name), `Json::LoadEntity` / `BuildEntity` and the `UMaps` name tables. |

## Utilities Folder (`header/Utilities/`)

| File | Description |
|------|-------------|
| `Random.hpp` | The `get_random<T>(min, max)` function template, built on `<random>`. |
| `Input.hpp` | The `get_input<T>()` function template (used for all player input) and `hold_input()`. |

## Systems Folder (`header/Systems/`)

General systems that drive a battle.

| File | Description |
|------|-------------|
| `BattlefieldSystem.hpp` | Battlefield events: domain interaction between fighters, dead characters and shikigami, spawning, and whether the fight can continue. |
| `CharacterSystem.hpp` | Applying damage to a character, the basic attack, and cloning characters (used when adding them to the battlefield). |
| `PlayerSystem.hpp` | The `UserControl` namespace: builds the player's action menu and handles each action. |
| `ResourceHandler.hpp` | Per-round ticking: stun, cursed energy, technique barrier upkeep, traits, shikigami, chants, amplification, RCT and output. |
| `SetupSystem.hpp` | The pre-fight setup menu: roster list, adding and removing fighters, choosing your character and the JSON toggle. |
| `Stringet.hpp` | A small header for string conversion (enum to string, `"X/Y"` formatting). |

### Mechanics Subfolder (`header/Systems/Mechanics/`)

| File | Description |
|------|-------------|
| `CombatSystem.hpp` | Resolves attacks, cursed tool attacks, technique usage and domain expansions, and calculates the damage actually taken. |
| `CursedToolSystem.hpp` | Held, stored and inventory weapon handling, and choosing a weapon by criteria (highest damage, effect inducing, technique bypassing, etc.). |
| `DomainSystem.hpp` | Domain surehit damage, clashing (refinement first, then attrition), upkeep cost and resetting. |
| `EffectSystem.hpp` | How status effects are applied and ticked (`ResourceHandler` uses it for ticking). |
| `NeutralizerSystem.hpp` | Neutralizer upkeep, being damaged and resetting (Simple Domain, Falling Blossom Emotion, etc.). |
| `ShikigamiSystem.hpp` | Shikigami damage, ticking, support boosts to the owner and shadow-state handling. |
| `SorcerySystem.hpp` | Cursed energy cost multipliers (efficiency, output status, Six Eyes, RCT, reinforcement) and burnout. |
| `TechniqueSystem.hpp` | Output and cursed energy checks, technique damage, ability selection, chants, and the barrier (upkeep cost, checks and toggling). |
| `VowSystem.hpp` | Binding vow generation, naming, cost calculation and application. |

## General Folder (`header/General/`)

Shared building blocks that are not tied to curse users.

| File | Description |
|------|-------------|
| `CursedTool.hpp` | The `CursedTool` struct. |
| `StatusEffects.hpp` | The `StatusEffect` struct plus `EffectType` and `EffectForType`. |

## Sorcery Folder (`header/Sorcery/`)

The structs that curse users own.

| File | Description |
|------|-------------|
| `BindingVow.hpp` | The `BindingVow` struct plus `SacrificeType`. |
| `Domain.hpp` | The `Domain` struct plus `DomainType`, `SurehitType` and `Refinement`. |
| `Neutralizer.hpp` | The `Neutralizer` struct plus `NeutralizerType`. |
| `Shikigami.hpp` | The `Shikigami` struct plus `SummonType`, `SupportType` and `ActiveAbility`. |
| `Technique.hpp` | The `Technique` struct alongside `TechAbility` and `TechBarrier`. |

## Source-Only Folders (`source/`)

| Folder | Description |
|--------|-------------|
| `Factories/` | The built-in content: one file per `Create::` namespace (`Characters.cpp`, `Techniques.cpp`, `Domains.cpp`, `Neutralizers.cpp`, `Shikigamis.cpp`, `CursedTools.cpp`, `StatusEffects.cpp`). |
| `Json/` | One loader per entity type, plus `UMaps.cpp` (the name tables that map JSON strings to built-in content and enums) and `Json.cpp`. |
| `main/` | `main.cpp`, which just runs `rungame()` in a loop. |

New `.cpp` files are picked up automatically: `CMakeLists.txt` globs `source/` recursively with `CONFIGURE_DEPENDS`.

---

# 2. Adding Characters

There are two routes: write a C++ factory (full control, compiled into the game), or write a JSON file (no recompiling).

## C++ Route

The game uses factories to create and configure characters, cursed tools, effects, etc. Adding a character takes three steps.

**Step 1: Declare it** in `header/Creator.hpp`:

```cpp
namespace Create {
    namespace Char {
        [[nodiscard]] Character MyChar();
        // ...
    }
    // ...
}
```

**Step 2: Define it** in `source/Factories/Characters.cpp`:

```cpp
Character Create::Char::MyChar() {
    Character c{};

    c.identity = {"My Character", "\x1b[38;5;117m", "A custom curse user."};

    c.state.max_health = 500.0;
    c.state.health     = c.state.max_health;
    c.state.strength   = 25.0;
    c.state.durability = 15.0;

    c.brain = Base::DecisionMaker{
        .targeting_type = TargetingType::HighestHP,
        .fighting_style = FightingStyle::Aggressive,
        .resource_usage = ResourceUsage::AllOut
    };

    c.equipment.has_access_to_inventory = true;
    c.equipment.current_tool = Create::Tool::PlayfulCloud();
    c.equipment.inventory.push_back(Create::Tool::SplitSoulKatana());

    // Everything below is what makes the character a curse user.
    // Leave it out entirely for a plain fighter like Toji or Maki.
    Sorcery::CurseUserSystem crs;
    crs.ce_system.max_cursed_energy   = 500.0;
    crs.ce_system.cursed_energy       = crs.ce_system.max_cursed_energy;
    crs.ce_system.regeneration_amount = 25.0;
    crs.output.max_output_potential   = 100.0;
    crs.jujutsu.technique   = Create::Tech::Limitless();
    crs.jujutsu.domain      = Create::Dmn::UnlimitedVoid();
    crs.jujutsu.neutralizer = Create::Nlr::SimpleDomain();
    crs.rct_system.can_use_rct = true;

    c.curseUser.emplace(std::move(crs));
    return c;
}
```

This gives the character 500 health, a held and an inventory cursed tool, the Limitless technique, Unlimited Void, Simple Domain and Reverse Cursed Technique. You do not have to follow this example strictly, you can set any values of your choice. Look at the existing characters in `Factories/Characters.cpp` for more.

> Keep `health` and `max_health` (and `cursed_energy` and `max_cursed_energy`) in sync when you set them, since both fields are stored.

**Step 3: Add it to the roster** in `SetupSystem::GetCharacterList` (`source/Systems/SetupSystem.cpp`):

```cpp
list.push_back(Create::Char::MyChar());
```

## JSON Route

The JSON route lets you create and configure characters without modifying any C++ code.

1. Create (or edit) `Characters.json` in the folder you run the game from. The repository root already contains a sample, and CMake copies it into `build/` when you configure.
2. Start the game and pick **Load JSON** in the setup menu. It toggles `[Custom Characters Enabled]`.
3. Your characters now appear at the end of the roster list.

A small `Characters.json` example:

```json
{
    "Characters": [
        {
            "Type": "Curse User",
            "Identity": {
                "Name": "My Character",
                "Color": "\u001b[38;5;117m",
                "Description": "A custom curse user."
            },
            "Stats": {
                "Health": 500.0,
                "Strength": 25.0,
                "Durability": 15.0,
                "Undefeatable": false
            },
            "Brain": {
                "Fighting Style": "Aggressive",
                "Resource Usage": "All Out",
                "Targeted Type": "Mixed",
                "Targeting Type": "Highest HP"
            },
            "Equipment": {
                "Held Tool": "Playful Cloud",
                "Stored Tool": "Split Soul Katana",
                "Inventory Access": true,
                "Inventory": ["Split Soul Katana"]
            },
            "Sorcery": {
                "Jujutsu": {
                    "Technique": "Limitless",
                    "Domain": "Unlimited Void",
                    "Neutralizer": "Simple Domain",
                    "Shikigami": ["Mahoraga"]
                },
                "Cursed Energy": {
                    "Amount": 500.0,
                    "Regen Amount": 25.0,
                    "Blackflash Chance": 5,
                    "Efficiency": "Peak"
                },
                "Cursed Energy Output": {
                    "Maximum": 100.0
                },
                "Domain Amplification": false,
                "Reverse Cursed Technique": true,
                "Traits": {
                    "Passive Healing": false,
                    "Six Eyes": true
                }
            }
        }
    ]
}
```

**Categories:**
- **Type:** Character type.
- **Identity:** Name, color, and description.
- **Stats:** Health, strength, and durability.
- **Brain:** AI behavior and targeting preferences.
- **Equipment:** Cursed tools and inventory.
- **Sorcery:** Curse-user abilities and resources, including:
    - **Jujutsu:** Techniques, domains, neutralizers, and shikigami.
    - **Cursed Energy:** Cursed energy amount, regeneration amount, efficiency, and Black Flash chance.
    - **Cursed Energy Output:** Maximum output potential.
    - **Traits:** Passive healing and Six Eyes.
    - **Other abilities:** Domain Amplification and Reverse Cursed Technique.

Every field is explained in the [JSON Reference](#3-json-reference).

---

# 3. JSON Reference

## 3.1 Files and Root Keys

Each entity type has its own file, and the file's array lives under a root key. All files are optional, and all of them must be in the folder you run the game from. The console prints `Looking for <file> in: <folder>` whenever one is read.

| File | Root key | Entity |
|------|----------|--------|
| `Characters.json` | `"Characters"` | Characters |
| `Techniques.json` | `"Techniques"` | Techniques and their abilities |
| `Domains.json` | `"Domains"` | Domains |
| `Neutralizers.json` | `"Neutralizers"` | Domain neutralizers |
| `Shikigami.json` | `"Shikigami"` | Shikigami |
| `CursedTools.json` | `"Cursed Tools"` | Cursed tools |
| `StatusEffects.json` | `"Status Effects"` | Status effects |

The repository root ships working samples of `Characters.json`, `Techniques.json` and `StatusEffects.json`: the *Limitless Sorcerer* character uses the *Limited* technique, whose *Yellow* ability inflicts the *Obliteration* status effect. It is a good example of entities referencing each other.

## 3.2 How Names Are Resolved

When a character says `"Technique": "Limited"`, the game looks the name up in this order:

1. **Built-in content** (the names in [section 4](#built-in-names-usable-from-json)).
2. **The matching JSON file**, comparing against each entry's `Identity` → `Name`.

Things to keep in mind:

- Names are **case sensitive** and must match exactly.
- A built-in name always wins, so a JSON entry cannot override a built-in with the same name.
- If a name matches nothing, that slot is simply left empty and no error is printed. If a technique, domain, tool or shikigami is missing in game, check the spelling first.
- Shikigami and inventory entries are arrays of names. Names that match nothing are skipped.

## 3.3 Defaults and Error Messages

The loader is forgiving. A missing key or a value of the wrong type prints a message like `"Health" couldn't be found. default values were applied` and uses the default, so loading never crashes on a missing field. Optional keys (such as `Stored Tool`) print the same message when omitted, which is harmless.

If a file is not valid JSON, the parse error is printed and **the whole file is ignored**. If you suddenly have no custom content, validate the file first.

## 3.4 Character Fields

### Type and Identity

| Key | Values | Default | Description |
|-----|--------|---------|-------------|
| `Type` | `"Curse User"`, `"Character"` | `"Character"` | `"Curse User"` enables the `Sorcery` section. `"Character"` is a plain fighter, and its `Sorcery` section is ignored. |
| `Identity` → `Name` | string | `"Delamain"` | Display name. |
| `Identity` → `Color` | ANSI escape string | `"\u001b[38;5;117m"` | Color the name is printed with. |
| `Identity` → `Description` | string | empty | Shown in the character list. |

### Stats

| Key | Type | Default | Description |
|-----|------|---------|-------------|
| `Health` | number | `50.0` | Starting and maximum health. |
| `Strength` | number | `25.0` | Damage dealt by basic attacks. |
| `Durability` | number | `15.0` | Reduces damage taken, with diminishing returns. |
| `Undefeatable` | bool | `false` | Makes the character invulnerable. |

> If the whole `Stats` object is left out, health, strength and durability all fall back to `1.0` instead.

### Brain (AI)

| Key | Values | Default | Description |
|-----|--------|---------|-------------|
| `Fighting Style` | `"Aggressive"`, `"Defensive"`, `"Mixed"` | `"Mixed"` | How the AI approaches a fight. |
| `Resource Usage` | `"All Out"`, `"Conservative"`, `"Mixed"` | `"Mixed"` | How freely it spends techniques, domains, shikigami and tools. |
| `Targeted Type` | `"Mixed"`, `"Character"` | `"Mixed"` | `"Mixed"` allows targeting characters and shikigami. Any other value means characters only. |
| `Targeting Type` | `"Highest HP"`, `"Lowest HP"`, `"Random"` | `"Random"` | Which target it prefers. |

### Equipment

| Key | Type | Description |
|-----|------|-------------|
| `Held Tool` | cursed tool name | The tool the character starts with in hand. |
| `Stored Tool` | cursed tool name | The tool the character starts with in the off-hand slot. |
| `Inventory Access` | bool | Whether the character may swap tools from the inventory during battle. |
| `Inventory` | array of cursed tool names | Tools the character carries. |

### Sorcery: Jujutsu

Only read for `"Type": "Curse User"`. Every value is looked up by name (see [3.2](#32-how-names-are-resolved)).

| Key | Type | Description |
|-----|------|-------------|
| `Technique` | technique name | The character's cursed technique. |
| `Domain` | domain name | The character's domain expansion. |
| `Neutralizer` | neutralizer name | The character's domain neutralizer. |
| `Shikigami` | array of shikigami names | Shikigami the character owns. |

### Sorcery: Cursed Energy and Output

| Key | Type | Default | Description |
|-----|------|---------|-------------|
| `Cursed Energy` → `Amount` | number | `500.0` | Starting and maximum cursed energy. |
| `Cursed Energy` → `Regen Amount` | number | `25.0` | Cursed energy regained per round. |
| `Cursed Energy` → `Blackflash Chance` | integer 0-100 | `5` | Percent chance of a Black Flash on a basic attack. Values outside 0-100 are clamped. |
| `Cursed Energy` → `Efficiency` | see below | `"Stable"` | Cursed energy cost multiplier tier. |
| `Cursed Energy Output` → `Maximum` | number | `100.0` | Maximum output potential. |

`Efficiency` values, from worst to best: `"Wasteful"`, `"Rough"`, `"Unstable"`, `"Stable"`, `"Precise"`, `"Expert"`, `"Extreme"`, `"Peak"`.

### Sorcery: Abilities and Traits

| Key | Type | Default | Description |
|-----|------|---------|-------------|
| `Domain Amplification` | bool | `false` | Whether the character can use Domain Amplification. |
| `Reverse Cursed Technique` | bool | `false` | Whether the character can use RCT. |
| `Traits` → `Passive Healing` | bool | `false` | Heals passively (the Cursed Spirits' trait). |
| `Traits` → `Six Eyes` | bool | `false` | Reduces cursed energy costs. |

## 3.5 Other Entity Fields

All of these entities share the same `Identity` block as characters (`Name`, `Color`, `Description`).

### Techniques (`Techniques.json`)

A technique is a name plus a list of abilities. The character's AI and the player choose between the abilities.

| Key | Type | Default | Description |
|-----|------|---------|-------------|
| `Identity` | object | | The technique's name, color and description. |
| `Barrier` | bool | `false` | **Top level**, next to `Identity`. Gives the technique a Limitless-style barrier that blocks incoming hits unless they bypass techniques (`Bypass Techniques` or `Bypass All`). A technique loaded with `true` starts with its barrier **active**. See the barrier upkeep note below. |
| `Abilities` | array | | The technique's abilities (below). |

**Barrier upkeep.** An active barrier costs cursed energy every round: `125 x the number of abilities` (62.5 if the technique has none). If the character cannot cover the cost, the barrier switches off by itself. Players can toggle it from the *Technique* menu (it needs enough cursed energy to cover one round of upkeep), and the AI turns it back on whenever it can afford to. Because the cost scales with the ability count, a technique with many abilities is expensive to keep protected.

Each entry in `Abilities` has its own `Identity` and a `Stats` object:

| Key (inside `Stats`) | Type | Default | Description |
|----------------------|------|---------|-------------|
| `Damage` | number | `0.0` | Base damage. |
| `Damage Type` | see [3.6](#36-accepted-values) | `"Normal"` | What the hit bypasses. |
| `Output` | number | `0.0` | Output the ability needs and adds. |
| `Cost` | number | `0.0` | Cursed energy cost. |
| `Inflicted Effect` | status effect name | none | Status effect applied on an unblocked hit. |

### Domains (`Domains.json`)

| Key | Type | Default | Description |
|-----|------|---------|-------------|
| `Type` | `"Open"`, `"Closed"` | `"Closed"` | **Top level**, not inside `Stats`. Open domains deal 2.5x damage to closed domains in a clash. |
| `Stats` → `Health` | number | `50.0` | Domain health, used up in clashes. |
| `Stats` → `Durability` | number | `50.0` | Divides the damage taken in a clash. |
| `Stats` → `Damage` | number | `50.0` | Surehit damage per round. |
| `Stats` → `Cost` | number | `50.0` | Cursed energy upkeep per round. |
| `Stats` → `Range` | integer | `75` | Domain range. In a clash, the damage you deal to the other domain is scaled by `their range / your range`. |
| `Stats` → `Damage Type` | see [3.6](#36-accepted-values) | `"Normal"` | What the surehit bypasses. |
| `Stats` → `Surehit Type` | see [3.6](#36-accepted-values) | `"Normal"` | How neutralizers interact with the surehit. |
| `Stats` → `Refinement` | see [3.6](#36-accepted-values) | `"Standard"` | The higher refinement wins a clash outright. Equal refinement is decided by attrition. |

### Neutralizers (`Neutralizers.json`)

| Key | Type | Default | Description |
|-----|------|---------|-------------|
| `Stats` → `Health` | number | `50.0` | Neutralizer health. |
| `Stats` → `Durability` | number | `50.0` | Reduces damage dealt to the neutralizer. |
| `Stats` → `Cost` | number | `50.0` | Cursed energy cost. |
| `Stats` → `Type` | `"Damage Reducing"`, anything else | fully protected | **Inside `Stats`** here (unlike domains). `"Damage Reducing"` lets surehit damage through at 1/2.5 of its value, unless the domain's surehit is `Basic`, which it nullifies. Anything else fully protects. |

### Shikigami (`Shikigami.json`)

| Key (inside `Stats`) | Type | Default | Description |
|----------------------|------|---------|-------------|
| `Health` | number | `100.0` | Starting and maximum health. |
| `Durability` | number | `10.0` | Reduces damage taken. |
| `Health Regeneration Speed` | number | `15.0` | Health regained per round. |
| `Cost` | number | `100.0` | Cursed energy cost to use. |
| `Support Type` | see [3.6](#36-accepted-values) | none | Which stat the shikigami boosts for its owner. |
| `Activation Ability` | see [3.6](#36-accepted-values) | none | Special ability while active. |


### Cursed Tools (`CursedTools.json`)

| Key (inside `Stats`) | Type | Default | Description |
|----------------------|------|---------|-------------|
| `Damage` | number | `50.0` | Damage dealt per hit. |
| `Damage Type` | see [3.6](#36-accepted-values) | `"Normal"` | What the hit bypasses. |
| `Inflicted Effect` | status effect name | none | Status effect applied on an unblocked hit (not stacked if the target already has it). |

### Status Effects (`StatusEffects.json`)

| Key (inside `Stats`) | Type | Default | Description |
|----------------------|------|---------|-------------|
| `Affected Stat` | see [3.6](#36-accepted-values) | none | The stat that changes each turn. |
| `Effect Type` | `"Regenerating"`, anything else | decreasing | `"Regenerating"` increases the stat each turn. Anything else decreases it. |
| `Effect Strength` | number | `0.0` | Amount applied each turn. |
| `Turn Amount` | integer | `3` | How many turns the effect lasts. |

## 3.6 Accepted Values

### Damage and Combat

| Setting | Accepted values | Default |
|---------|-----------------|---------|
| `Damage Type` | `"Normal"`, `"Bypass Techniques"`, `"Bypass Reinforcement"`, `"Bypass All"` | `"Normal"` |

What each damage type does:

| Damage Type | Technique barriers (Limitless) | Cursed energy reinforcement | Durability |
|-------------|--------------------------------|-----------------------------|------------|
| `Normal` | Block it | Reduces it | Reduces it |
| `Bypass Techniques` | Ignored | Reduces it | Reduces it |
| `Bypass Reinforcement` | Block it | Ignored | Reduces it |
| `Bypass All` | Ignored | Ignored | Ignored |

### Domain Settings

| Setting | Accepted values | Default |
|---------|-----------------|---------|
| `Refinement` (weakest to strongest) | `"Brittle"`, `"Weak"`, `"Standard"`, `"Adept"`, `"Overwhelming"`, `"Absolute"` | `"Standard"` |
| `Surehit Type` | `"Basic"`, `"Normal"`, `"Paralyzing"` | `"Normal"` |

What each surehit type does against a neutralizer:

| Surehit Type | Behavior |
|--------------|----------|
| Basic | Neutralizers nullify the domain, even with damage reduction. |
| Normal | Neutralizers can withstand the domain but cannot fully nullify it. |
| Paralyzing | Neutralizers must actually neutralize the surehit, or the target is paralyzed (stunned). |

Any other value falls back to `"Normal"`.

### Shikigami Settings

| Setting | Accepted values | Default |
|---------|-----------------|---------|
| `Support Type` | `"Strength"`, `"Durability"`, `"Cursed Energy Regeneration"`, `"Output"` | none |
| `Activation Ability` | `"Technique Bypassing"` | none |

### Status Effect Settings

| Setting | Accepted values | Default |
|---------|-----------------|---------|
| `Affected Stat` | `"Health"`, `"Durability"`, `"Strength"`, `"Cursed Energy"`, `"Max Output"` | none (the effect does nothing) |

## 3.7 Full Example Set

A small mod where every file references the others. Put all of these next to the game, enable **Load JSON**, and *Test Sorcerer* appears in the roster with every piece resolved.

<details>
<summary><code>Characters.json</code></summary>

```json
{
    "Characters": [
        {
            "Type": "Curse User",
            "Identity": { "Name": "Test Sorcerer", "Color": "\u001b[38;5;117m", "Description": "A modded sorcerer." },
            "Stats": { "Health": 800.0, "Strength": 60.0, "Durability": 40.0, "Undefeatable": false },
            "Brain": {
                "Fighting Style": "Defensive",
                "Resource Usage": "Conservative",
                "Targeted Type": "Character",
                "Targeting Type": "Lowest HP"
            },
            "Equipment": {
                "Held Tool": "Venom Dagger",
                "Inventory Access": true,
                "Inventory": ["Venom Dagger", "Playful Cloud"]
            },
            "Sorcery": {
                "Jujutsu": {
                    "Technique": "Ember Arts",
                    "Domain": "Hollow Furnace",
                    "Neutralizer": "Ash Veil",
                    "Shikigami": ["Cinder Hound"]
                },
                "Cursed Energy": { "Amount": 3000.0, "Regen Amount": 90.0, "Blackflash Chance": 10, "Efficiency": "Expert" },
                "Cursed Energy Output": { "Maximum": 120.0 },
                "Domain Amplification": true,
                "Reverse Cursed Technique": false,
                "Traits": { "Passive Healing": true, "Six Eyes": false }
            }
        }
    ]
}
```
</details>

<details>
<summary><code>Techniques.json</code></summary>

```json
{
    "Techniques": [
        {
            "Identity": { "Name": "Ember Arts", "Color": "\u001b[38;5;202m", "Description": "Sparks and cinders." },
            "Barrier": false,
            "Abilities": [
                {
                    "Identity": { "Name": "Spark", "Color": "\u001b[38;5;208m", "Description": "A small burst." },
                    "Stats": { "Damage": 40.0, "Output": 20.0, "Cost": 60.0 }
                },
                {
                    "Identity": { "Name": "Cinder Lance", "Color": "\u001b[38;5;196m", "Description": "Pierces reinforcement." },
                    "Stats": {
                        "Damage": 150.0,
                        "Damage Type": "Bypass Reinforcement",
                        "Output": 90.0,
                        "Cost": 400.0,
                        "Inflicted Effect": "Scorch"
                    }
                }
            ]
        }
    ]
}
```
</details>

<details>
<summary><code>Domains.json</code></summary>

```json
{
    "Domains": [
        {
            "Identity": { "Name": "Hollow Furnace", "Color": "\u001b[38;5;202m", "Description": "A furnace of nothing." },
            "Type": "Open",
            "Stats": {
                "Health": 400.0,
                "Durability": 60.0,
                "Damage": 90.0,
                "Cost": 250.0,
                "Range": 80,
                "Damage Type": "Bypass Techniques",
                "Surehit Type": "Basic",
                "Refinement": "Adept"
            }
        }
    ]
}
```
</details>

<details>
<summary><code>Neutralizers.json</code></summary>

```json
{
    "Neutralizers": [
        {
            "Identity": { "Name": "Ash Veil", "Color": "\u001b[38;5;250m", "Description": "A veil of ash." },
            "Stats": { "Health": 300.0, "Durability": 50.0, "Cost": 40.0, "Type": "Damage Reducing" }
        }
    ]
}
```
</details>

<details>
<summary><code>Shikigami.json</code></summary>

```json
{
    "Shikigami": [
        {
            "Identity": { "Name": "Cinder Hound", "Color": "\u001b[38;5;166m", "Description": "A hound made of embers." },
            "Stats": {
                "Health": 300.0,
                "Durability": 35.0,
                "Health Regeneration Speed": 20.0,
                "Cost": 150.0,
                "Support Type": "Strength",
                "Activation Ability": "Technique Bypassing"
            }
        }
    ]
}
```
</details>

<details>
<summary><code>CursedTools.json</code></summary>

```json
{
    "Cursed Tools": [
        {
            "Identity": { "Name": "Venom Dagger", "Color": "\u001b[38;5;22m", "Description": "A dagger coated in something nasty." },
            "Stats": { "Damage": 70.0, "Damage Type": "Bypass Reinforcement", "Inflicted Effect": "Scorch" }
        }
    ]
}
```
</details>

<details>
<summary><code>StatusEffects.json</code></summary>

```json
{
    "Status Effects": [
        {
            "Identity": { "Name": "Scorch", "Color": "\u001b[38;5;202m", "Description": "Burns every turn." },
            "Stats": { "Affected Stat": "Health", "Effect Type": "Draining", "Effect Strength": 30.0, "Turn Amount": 4 }
        }
    ]
}
```
</details>

---

# 4. Adding Other Content in C++

Techniques, domains, neutralizers, shikigami, cursed tools and status effects all follow the same recipe as characters:

| Content | Namespace in `Creator.hpp` | Define in `source/Factories/` | Register for JSON in `source/Json/UMaps.cpp` |
|---------|----------------------------|-------------------------------|----------------------------------------------|
| Techniques | `Create::Tech` | `Techniques.cpp` | `GetTCMap()` |
| Domains | `Create::Dmn` | `Domains.cpp` | `GetDMMap()` |
| Neutralizers | `Create::Nlr` | `Neutralizers.cpp` | `GetNLMap()` |
| Shikigami | `Create::Shk` | `Shikigamis.cpp` | `GetSKMap()` |
| Cursed Tools | `Create::Tool` | `CursedTools.cpp` | `GetCTMap()` |
| Status Effects | `Create::Effect` | `StatusEffects.cpp` | `GetSTMap()` |

1. **Declare** the factory in the matching namespace of `Creator.hpp`.
2. **Define** it in the matching file in `source/Factories/`.
3. **Use it** directly from C++ (for example `crs.jujutsu.technique = Create::Tech::MyTechnique();`).
4. **Optionally register it** in `UMaps.cpp` so JSON files can refer to it by name. An entry looks like `{"My Technique", Create::Tech::MyTechnique()}`.

An example technique factory:

```cpp
Technique Create::Tech::MyTechnique() {
    Technique c{};

    const EntityInfo id       = {"My Technique", "\x1b[38;5;45m", "A custom technique."};
    const EntityInfo slash_id = {"Slash", "\x1b[38;5;9m", "A quick slash."};

    c.identity = id;

    // damage, cost, output
    const TechAbility slash = {slash_id, {}, 75.0, 35.0, 17.5};
    c.abilities.push_back(slash);

    // true allows a Limitless-style barrier. It starts switched off unless you also
    // set c.barrier.is_active = true (Limitless does). Active barriers cost cursed
    // energy every round, see TechniqueSystem::GetBarrierCost.
    c.barrier.can_use_barrier = false;
    return c;
}
```

`TechAbility` fields, in order: `id`, `given_effect`, `damage`, `cost`, `output`, `chant_amount`, `damage_type`. Look at `Factories/Techniques.cpp` for more elaborate examples.

## Built-in Names Usable from JSON

These are the names currently registered in `UMaps.cpp`:

| Category | Names |
|----------|-------|
| Techniques | `Limitless`, `Shrine`, `Idle Transfiguration`, `Disaster Plants`, `Disaster Flames` |
| Domains | `Unlimited Void`, `Malevolent Shrine`, `Self Embodiment Of Perfection`, `Coffin Of The Iron Mountain`, `Shining Sea Of Growing Branches` |
| Neutralizers | `Simple Domain`, `Hollow Wicker Basket`, `Falling Blossom Emotion` |
| Shikigami | `Mahoraga`, `Agito`, `Ember Insects` |
| Cursed Tools | `Split Soul Katana`, `Inverted Spear of Heaven`, `Playful Cloud` |
| Status Effects | `Bleed` (its in-game display name is "Bleeding") |

Every technique and domain declared in `Creator.hpp` is registered. If you add your own factory and want JSON to be able to use it by name, add it to the matching getter in `UMaps.cpp` (see the table above).

---

# 5. Testing Your Mod

- **Run the game from the folder that contains your JSON files.** The game reads files from the current working directory, and prints where it is looking.
- **New JSON files in the repository root** are copied into `build/` when CMake configures, so re-run `cmake -S . -B build` after adding one. Edits to the files that were already copied also trigger the copy on the next build. Or just drop the file into the folder you run from.
- **Remember to toggle Load JSON** in the setup menu, otherwise only the built-in roster is shown.
- **Use View Character Information** in the setup menu to check that technique, domain and tool names all resolved.

### Troubleshooting

| Symptom | Likely cause |
|---------|--------------|
| `Could not find Characters.json!` | You are running the game from a different folder than the one containing the file. |
| `JSON Parse Error: ...` | The file is not valid JSON (trailing comma, missing quote, etc.). The whole file is ignored. |
| Custom characters do not appear | **Load JSON** is not toggled on, or `"Characters"` is misspelled as the root key. |
| A technique, domain, tool or shikigami is missing in game | The name does not match exactly (names are case sensitive), or it refers to your own C++ factory that is not registered in `UMaps.cpp`. |
| `"X" couldn't be found. default values were applied` | The key is missing or misspelled. The default is used and the game continues. |
| `"X" has an invalid value type` | The value has the wrong JSON type, for example a string where a number is expected. |
| Character has 1 health and 1 strength | The `Stats` object is missing or misspelled. |
