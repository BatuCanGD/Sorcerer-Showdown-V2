<p align="center">
  <img src="https://img.shields.io/badge/LINES%20OF%20CODE-4880-blue?style=for-the-badge" />
  <img src="https://img.shields.io/badge/FILES-73-yellow?style=for-the-badge" />
</p>

# Sorcerer Showdown V2

A turn-based battle simulator set in the Jujutsu Kaisen universe, played in the terminal and written in C++23.

**This is a project that I have been thinking about for a long time. I have been wanting to really rework the systems and remove the clutter that the old project carries, and improve the project long term with my current C++ knowledge.**

## Features

- **Free-for-all battles:** put as many fighters on the battlefield as you like, then either take control of one or sit back and watch the AI fight it out.
- **Cursed energy and output:** every technique costs cursed energy *and* output. Push too hard and you burn out. Land a Black Flash and your output gets boosted.
- **Techniques with depth:** chant an ability for bonus damage, use finishers that need a chant level or a health range, and fire area-of-effect abilities across the battlefield.
- **Domain Expansions:** open and closed domains with refinement levels and different surehit types. Clashing domains are decided by refinement first, then by attrition.
- **Domain neutralizers:** Simple Domain, Hollow Wicker Basket, Falling Blossom Emotion.
- **Shikigami:** summoned as shadows, support or active fighters, with support boosts and special abilities.
- **Binding Vows:** trade health, cursed energy or output potential for another of the three.
- **Cursed tools and status effects:** weapons that can bypass techniques or reinforcement and inflict effects over time.
- **Configurable AI:** each character has its own fighting style, resource usage and targeting preferences.
- **Moddable:** add characters, techniques, domains and more in C++ or with plain JSON files. See [MODDING.md](MODDING.md).

## Current Roster

| Character | Type | Kit |
|-----------|------|-----|
| Satoru Gojo | Curse User | Limitless (Blue, Red, Purple, Unlimited Hollow Purple), Unlimited Void, Simple Domain, Six Eyes, Reverse Cursed Technique |
| Ryomen Sukuna | Curse User | Shrine (Dismantle, Cleave, Spiderweb, World Cutting Slash), Malevolent Shrine, Hollow Wicker Basket, Mahoraga and Agito shikigami, high CE pool |
| Mahito | Cursed Spirit | Idle Transfiguration, Self-Embodiment of Perfection, Passive Healing |
| Jogo | Cursed Spirit | Disaster Flames, Coffin of the Iron Mountain, Passive Healing |
| Hanami | Cursed Spirit | Disaster Plants, Shining Sea of Growing Branches, Passive Healing |
| Maki Zenin | Heavenly Restricted | Glass cannon with the Split Soul Katana |
| Toji Fushiguro | Heavenly Restricted | Diverse cursed tools (Inverted Spear of Heaven, Split Soul Katana, Playful Cloud) |

Custom characters loaded from JSON appear in the same list once enabled (see [How to Play](#how-to-play)).

## Getting Started

### Requirements

- **CMake 3.28** or newer
- A C++23 compiler **with `<print>` support**: GCC 14+, a recent MSVC (Visual Studio 2022 17.7+), or a recent Clang with a matching standard library. GCC 13 and older will fail on `#include <print>`.
- An internet connection on the first configure. [nlohmann/json](https://github.com/nlohmann/json) (v3.11.3) is downloaded automatically with CMake's `FetchContent`.

### Build

```bash
git clone https://github.com/batucangd/sorcerer-showdown-v2.git
cd sorcerer-showdown-v2

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

### Run

**Run the game from inside the `build` folder.** The game looks for a `Json` folder in the current working directory. CMake copies the repository's `Json/` folder to `build/Json/` when you configure, so re-run the configure step (`cmake -S . -B build`) after you change a file in `Json/`. A normal build does not copy them again.

```bash
cd build
./SorcererShowdownV2            # Linux / macOS (single-config generators)
.\Release\SorcererShowdownV2.exe  # Windows / Visual Studio (multi-config generators)
```

## How to Play

### 1. Setup menu

Before the fight starts you build the battlefield. Every option is picked by typing its number.

| Option | What it does |
|--------|--------------|
| Start | Begins the fight (needs at least 2 fighters) |
| Add Character / Add Multiple | Adds one or several copies of a character from the roster |
| Choose and Add Character | Adds a character **and makes it yours** |
| Choose from Battlefield | Takes control of a fighter that is already on the battlefield |
| Remove Specific / Last / Chosen Character | Removes fighters, or releases your chosen one |
| Clear Battlefield | Removes everyone |
| View Character Information | Shows the stats, technique, domain and tool of a fighter |
| Load JSON | Toggles custom characters from your JSON files on or off (shown as `[Custom Characters Enabled]`) |

If you never choose a character, every fighter is controlled by the AI and the match plays out on its own.

After starting, you pick how much of the battle to skip: everything, up to the end of each round, or nothing.

### 2. Your turn

On your turn you are offered the actions your character can actually use:

| Action | Available when |
|--------|----------------|
| Attack | Always |
| Inventory | The character holds or stores a cursed tool, or has inventory access with tools in it |
| Status Effects | The character currently has at least one status effect |
| Technique, Domain, Shikigami | The character is a curse user and owns that ability |
| Sorcery | Curse users: Reverse Cursed Technique, Reinforcement, Binding Vows, Domain Amplification (only the options that apply to the character are listed) |

At the end of every round, cursed energy regenerates, effects and shikigami tick, domains clash or hit, and dead fighters are removed. **The last fighter standing wins.**

### 3. Combat notes

- Durability has diminishing returns: a target never takes less than 10% of an incoming hit from durability alone.
- **Damage types** decide what a hit ignores: `Normal`, `BypassTech` (technique barriers such as Limitless), `BypassRein` (cursed energy reinforcement) and `BypassAll` (both, plus durability).
- A **Black Flash** deals 2.5x damage and its chance is set per character.

### 4. Techniques

A technique is a set of abilities, and every ability has its own damage, cursed energy cost and output.

- **Chanting:** choose *Chant* on an ability in the Technique menu to build up its chant level (+1 per chant, up to 5). While the level is above 0, the ability's damage is multiplied by `1 + chant / 2`, so level 4 deals 3x and level 5 deals 3.5x. Chant levels fall by 1 every round, and an ability's chant level resets to 0 once you use it. The AI chants on its own, up to twice per turn.
- **Requirements:** an ability can require a minimum chant level, a health range, or both. *High Health* means at least 75% of max health, *Mid Health* means between 25% and 75%, and *Low Health* means 25% or less. Abilities you don't currently qualify for are marked *Requirements not met* in the menu.
- **Area of Effect:** these abilities hit every other character on the battlefield instead of a single target. The user is never hit by their own attack, and shikigami are not targeted.
- **Barrier:** some techniques, such as Limitless, have a barrier that blocks incoming hits unless they bypass techniques. It costs cursed energy every round (125 CE per ability the technique has, before the spending multiplier) and can be switched on or off from the Technique menu. If the character can't pay, the barrier switches off and their remaining cursed energy is spent.
- Gojo's *Unlimited Hollow Purple* (Area of Effect) and Sukuna's *World Cutting Slash* both require chant level 4.

### 5. Cursed energy spending

Every curse user pays a **spending multiplier** on the cursed energy they use. It comes from their **Efficiency** tier, and the Six Eyes trait stacks on top of it.

| Efficiency | Cost multiplier |
|------------|-----------------|
| Wasteful | 2.25x |
| Rough | 1.75x |
| Unstable | 1.30x |
| Stable (default) | 1.00x |
| Precise | 0.80x |
| Expert | 0.70x |
| Extreme | 0.55x |
| Peak | 0.40x |

- **Six Eyes** multiplies the cost by a further 0.05x. For example, a 400 CE ability costs `400 x 0.40 x 0.05 = 8` CE for a Peak character with Six Eyes.
- The multiplier applies to technique abilities, domain upkeep, neutralizer upkeep, technique barrier upkeep, Domain Amplification upkeep, Reverse Cursed Technique and Reinforcement. The AI uses the multiplied cost when it decides whether it can afford something.
- Efficiency also scales how much **output** a technique ability adds (Six Eyes does not).
- **Reinforcement** and **Reverse Cursed Technique** are paid for every round, before cursed energy regenerates, and only while they are set above 0 (characters start with no reinforcement). If a character can't afford the cost, all of their remaining cursed energy is spent and the amount resets to 0, so RCT heals nothing that round and reinforcement drops. The menus warn you with *Will consume all of your Cursed Energy* before you confirm. While it lasts, reinforcement counts toward durability at 1/1.75 of its value, unless the hit bypasses reinforcement.
- Binding Vows use their own cost formula and are not scaled by the multiplier.

## Project Structure

```
sorcerer-showdown-v2/
├── CMakeLists.txt
├── Json/                   # sample JSON mod files, copied to build/Json at configure time
│   ├── Characters.json
│   ├── Techniques.json
│   ├── Domains.json
│   ├── Neutralizers.json
│   ├── Shikigami.json
│   ├── CursedTools.json
│   └── StatusEffects.json
├── MODDING.md              # full guide for modding the game
└── Sorcerer Showdown V2/
    ├── header/             # all headers (core structs, systems, utilities)
    └── source/
        ├── Factories/      # built-in characters, techniques, domains, etc.
        ├── Json/           # JSON loading for every entity type
        ├── Systems/        # battle, setup, player and mechanics systems
        └── main/main.cpp   # entry point
```

A `Character` is built from composition (identity, state, equipment, AI brain and an optional curse user component). The full breakdown of every header and system is in [MODDING.md](MODDING.md#1-project-structure).

## Modding

To learn how to mod the game, including adding characters, techniques, domains, shikigami, cursed tools and status effects through C++ or JSON, refer to [MODDING.md](MODDING.md).
