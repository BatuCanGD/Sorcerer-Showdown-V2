# Modding Introduction

Welcome to MODDING.md for Sorcerer-Showdown-V2, This will be a lengthy guide on how to modify the game yourself, and if it ever happens in the future, How to use JSON files to add custom Items/Characters

---

## Current Project Structure

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
| `Technique.hpp` | includes the Technique struct alongside with its TechAbility struct |

---

## CharacterType Folder

Includes the classes the game iterates on

| File | Description |
|------|-------------|
| `Character.hpp` | includes: Stats, Cursed Tools and inventory and the BattleIQ struct that contains enums included in AI.hpp for the AI specifically |
| `CurseUser.hpp` | includes: Cursed Energy Systems, RCT systems, Jujutsu (Technique, domain, etc..) Systems, Cursed Energy Output Systems and a Trait System |


# Adding Characters Guide

In this part of the documentation, there will be details on how you will be able to add new custom characters and on how to customize them.

### C++ Route

This game uses factories to create and configure characters, cursed tools, effects, etc...

a small example on how to create a custom curse user character type would look like this:

```` cpp
// how the implementation should look like in the hpp file
namespace Create {
    namespace Char {
        CurseUser MyChar(); // the character type and who it will create
        // .....
    }
    // ....
}

// how the implementation should look like in the cpp file
CurseUser Create::MyChar() {
    CurseUser c{};
    c.State().max_health = 500.0;
    c.State().health = 500.0;
    c.Jujutsu().technique = Create::Tech::Limitless();
    return c; 
}

````
sets the characters health to 500.0 and gives the character the limitless technique \
you dont have to follow this example strictly, you can add any value of your choice

### JSON Route

**### JSON Route**

The JSON route allows you to create and configure characters without modifying C++ code.

a small json file example would look like this:

```json
{
    "Characters": [
        {
            "Type": "Curse User",
            "Identity": {
                "Name": "My Character",
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
    - **Cursed Energy:** Cursed Energy Amount, regeneration amount, efficiency, and Black Flash chance.
    - **Cursed Energy Output:** Maximum output potential.
    - **Traits:** Passive healing and Six Eyes.
    - **Other abilities:** Domain Amplification and Reverse Cursed Technique.