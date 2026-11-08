# Changelog

All notable project and repository changes are recorded here.

## 2026-11-08 — Runtime probe verified

- Recorded the owner's successful in-game tests of the read-only runtime probe across two game runs.
- Confirmed runtime values for the VC 1.0 main module, menu manager, MAP page, mouse coordinates, and show-mouse flag.
- Documented observed page transitions and explicitly left transient page value `32` unexplained.
- Updated project status: the next milestone is safe draw-hook and native-font research, followed by a fixed MAP-only text overlay.
- No production drawing hook, map replacement, or asset modification was introduced.

## 2026-11-01 — Project formalization

- Renamed the repository to **Vice-City-Pause-Map-Expansion**.
- Updated the repository README from a temporary build-test description to the formal project overview.
- Renamed the GitHub Actions workflow display name and build artifact to match the project.
- Added a project status/verification document and this changelog.
- Preserved the existing minimal ASI load-probe source; no game hooks or map modifications were introduced.

### Verified before this documentation update

- Windows x86 MSVC workflow completed successfully.
- The produced binary was verified as PE x86 (machine `0x014c`).
- The user manually confirmed that the ASI load-test message appeared in GTA Vice City.

### Not yet verified / not implemented

- Pause-menu MAP page overlay.
- Stable mouse-to-map coordinate conversion.
- Zone-name hover display.
- Right-click waypoint placement.
