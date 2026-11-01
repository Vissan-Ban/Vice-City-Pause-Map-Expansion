# Vice City Pause Map Expansion

A lightweight ASI plugin project for **Grand Theft Auto: Vice City PC 1.0 (32-bit)**.

The goal is to expand the original pause-menu **MAP** page with mouse-driven location labels and waypoint marking while preserving the vanilla map renderer, textures, and game assets.

## Project principles

- Preserve the original pause-menu map rendering and all vanilla map assets.
- Do not replace map textures or introduce IPL/map-data changes.
- Keep the plugin lightweight and avoid unrelated gameplay changes.
- Add features incrementally, with each step independently testable.
- Prefer stable, reversible hooks and document every verified change.

## Planned development sequence

1. **ASI loading and build pipeline** — establish a repeatable Windows x86 MSVC build and verify that the game loads the plugin.
2. **Map-page overlay probe** — display a fixed test label only on the original MAP page, without replacing map drawing.
3. **Mouse coordinate mapping** — determine reliable map-relative coordinates.
4. **Location hover labels** — map the pointer position to the game's original zone/location names.
5. **Right-click waypoint marker** — add a map marker using the original game's coordinate and waypoint mechanisms where feasible.
6. **Release preparation** — test compatibility, document installation/removal, and publish only verified builds.

## Current status

The initial 32-bit ASI load probe has been built successfully with GitHub Actions and manually confirmed to load in-game. This proves the loader/build path only; it does **not** yet prove any map-page hook or map interaction.

The current `src/probe.cpp` is intentionally a harmless load-test plugin. It does not access game addresses, patch instructions, hook functions, or alter game data.

See [CHANGELOG.md](CHANGELOG.md) for the project record and [docs/PROJECT_STATUS.md](docs/PROJECT_STATUS.md) for the current verification checklist.

## Build

Pushes to `main` and manual workflow dispatch run the Windows x86 MSVC build:

- Workflow: [Build VC ASI](.github/workflows/build.yml)
- Target: 32-bit x86 DLL-style ASI
- Output: `vc_pause_map_expansion_probe.asi` (workflow artifact)

## Compatibility scope

- Game: Grand Theft Auto: Vice City
- Executable: PC 1.0
- Architecture: 32-bit x86
- Current stage: ASI load probe only

No map functionality is claimed until it has been implemented and tested in-game.
