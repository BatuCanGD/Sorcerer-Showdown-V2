# Modding Introduction

Welcome to MODDING.md for Sorcerer-Showdown-V2, This will be a lengthy guide on how to modify the game yourself, and if it ever happens in the future, How to use JSON files to add custom Items/Characters

---

## Current Project Structure (Extra Details)

### Root Files

| File | Description |
|------|-------------|
| `Game.hpp` | Includes the game loop, the setting up of the battlefield vector and the handling of the end of the program. |
| `AI.hpp` | Includes the AI's decision making Enums and its namespace functions. |
| `Logger.hpp` | Includes many types of logging to the console/terminal. |
| `Creator.hpp` | Includes the creation factories including: Characters, Techniques, Domains, Neutralizers, Cursed tools and Status Effects. |
| `Enums.hpp` | Includes the globalums namespace that only includes the DamageType enum. |
| `Battlefield.hpp` | Includes the Battlefield struct including the battlefield vector and the spawn_next vector that has the characters in the vector moved into the battlefield vector next turn of the game. |

---

## Utilities Folder

Includes small utilities

| File | Description |
|------|-------------|
| `Random.hpp` | includes the get_random function template that uses the `<random>` library. |
| `Input.hpp` | includes the get_input function template. Used for player input. |

---

## Systems Folder

Includes the headers of general systems.

| File | Description |
|------|-------------|
| `BattlefieldSystem.hpp` | handles battlefield events like domains or character deaths/spawns. |
| `PlayerSystem.hpp` | handles player input and the player character side of things. |
| `ResourceHandler.hpp` | handles status/state ticking. |
| `SetupSystem.hpp` | handles the battlefield vector setup. |
| `Stringet.hpp` | very weird header but used for string conversion and enum to string types. |

### Mechanics SubFolder

| File | Description |
|------|-------------|
| `CombatSystem.hpp` | handles damage and combat handling (Domains, Techniques, etc...) between systems |
| `DomainSystem.hpp` | handles domain clashing, domain damaging and resetting functions |
| `EffectSystem.hpp` | handles how effects are applied and ticking (ResourceHandler uses EffectSystem's function for ticking) |
| `NeutralizerSystem.hpp` | handles domain neutralizers like simple domain and falling blossom emotion, same with domain, handles resetting and being damaged too |
| `ShikigamiSystem.hpp` | handles shikigami effects and boosts, has actual components like being disabled if their health is less than 0 |
| `SorcerySystem.hpp` | mostly handles costs like the six eyes multiplier, rct cost handler and reinforcement for curse user class types |
| `VowSystem.hpp` | handles binding vows, equal exchange and binding vow generation alongside with naming |

---

## General Folder

Includes only Cursed Tools and Status Effect header files

| File | Description |
|------|-------------|
| `CursedTools.hpp` | includes the CursedTool struct |
| `StatusEffects.hpp` | includes the StatusEffect struct plus its enums |

---

## Sorcery Folder

Includes header files related to CurseUser class Characters

| File | Description |
|------|-------------|
| `BindingVow.hpp` | includes the BindingVow struct plus its enum |
| `Domain.hpp` | includes the Domain struct plus its enums |
| `Neutralizer.hpp` | includes the Neutralizer struct plus its enums |
| `Shikigami.hpp` | includes the Shikigami struct plus its enums |
| `Technique.hpp` | includes the Technique class alongside with its TechAbility struct |

---

## CharacterType Folder

Includes the classes the game iterates on

| File | Description |
|------|-------------|
| `Character.hpp` | includes: Stats, Cursed Tools and inventory and the BattleIQ struct that contains enums included in AI.hpp for the AI specifically |
| `CurseUser.hpp` | includes: Cursed Energy Systems, RCT systems, Jujutsu (Technique, domain, etc..) Systems, Cursed Energy Output Systems and a Trait System |