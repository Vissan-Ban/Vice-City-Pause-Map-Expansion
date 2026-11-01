# Changelog

All notable project and repository changes are recorded here.

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
