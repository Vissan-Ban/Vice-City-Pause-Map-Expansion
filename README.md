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

- [x] Runtime probe confirmed the VC 1.0 menu manager, MAP page, and mouse fields in-game.
- [x] Separate fixed-text ASI built by GitHub Actions and **visually confirmed by the project owner** on the original MAP page.
- [ ] Lower-right text placement is the current positioning experiment; bottom-row clearance still awaits in-game confirmation.
- [ ] Mouse-to-map/world-coordinate conversion and hover location labels are not yet implemented.
- [ ] Waypoint placement remains deferred.

The fixed-text hook queues a CFont string before the game's original `CFont::DrawFonts` call. It does not replace the original MAP renderer at `0x0049A5B7`. The current build is an experimental test artifact, not a finished release.

See [CHANGELOG.md](CHANGELOG.md) for the project record and [docs/PROJECT_STATUS.md](docs/PROJECT_STATUS.md) for the verification checklist.

## Build

Pushes to `main` and manual workflow dispatch run the Windows x86 MSVC build:

- Workflow: [Build VC ASI](.github/workflows/build.yml)
- Target: 32-bit x86 DLL-style ASI
- Output: `vc_pause_map_expansion_probe.asi` (workflow artifact)

## Compatibility scope

- Game: Grand Theft Auto: Vice City
- Executable: PC 1.0
- Architecture: 32-bit x86
- Current stage: experimental fixed-text overlay; placement iteration in progress

Only the fixed-text overlay has been visually verified. Mouse coordinate conversion, hover labels, and waypoint behavior are not yet claimed.
