<p align="center">
  <img src="https://img.shields.io/badge/LINES%20OF%20CODE-2808-blue?style=for-the-badge" />
  <img src="https://img.shields.io/badge/FILES-54-yellow?style=for-the-badge" />
</p>

# Sorcerer Showdown V2 (Unfinished)

**This is a project that i have been thinking about for a long time, i have been wanting to really rework the systems and remove clutter that the old project carries, and improve the project long term with my current C++ knowledge.**

## Current Project Structure

Basic overview of the project structure, for more detail look at [MODDING.md](MODDING.md)

### Root Files

| File | Description |
|------|-------------|
| `Game.hpp` | Includes the game loop |
| `AI.hpp` | Includes the AI decision making functions |
| `Logger.hpp` | Includes many types of logging functions |
| `Creator.hpp` | Includes creation factories |
| `Enums.hpp` | Includes enums that are shared across many places |
| `Battlefield.hpp` | Includes the Battlefield struct |

### Utilities Folder

Includes small utilities

### Systems Folder

Includes the headers of general systems.

| File | Description |
|------|-------------|
| `BattlefieldSystem.hpp` | handles battlefield events |
| `PlayerSystem.hpp` | handles player input |
| `ResourceHandler.hpp` | handles status/state ticking. |
| `SetupSystem.hpp` | handles the battlefield vector setup. |
| `Stringet.hpp` | used for string conversion and enum-to-string types. |

#### Mechanics SubFolder

| File | Description |
|------|-------------|
| `CombatSystem.hpp` | handles damage between systems |
| `DomainSystem.hpp` | handles domain clashes, etc.. |
| `EffectSystem.hpp` | handles how effects are applied |
| `NeutralizerSystem.hpp` | handles neutralizers that nullify/reduce the effects of a domain |
| `ShikigamiSystem.hpp` | handles shikigami effects and boosts |
| `SorcerySystem.hpp` | handles cursed energy costs |
| `VowSystem.hpp` | handles binding vows |

### General Folder

Includes only Cursed Tools and Status Effect header files

### Sorcery Folder

Includes base header files related to CurseUser class Characters

### CharacterType Folder

Includes the Character classes

## To build the Project

To build the project, using CMake with a compiler that has the libraries and support for c++23 is recommended \
Opening a terminal in the project directory is required to properly compile the project and for cmake to work properly

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
# Configures the project and generates the build system using the Release configuration
cmake --build build
# builds the project
./build/SorcererShowdownV2
# runs the project
```

## Modding

To learn how to mod the game refer to [MODDING.md](MODDING.md)