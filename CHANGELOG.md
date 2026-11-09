# Changelog

All notable project and repository changes are recorded here.

## 2026-11-09 — Native font API mapped

- Added VC-specific CFont API research with function addresses, cdecl signatures, and the wide-character print path.
- Documented that ASI should call the game's CFont routines directly rather than trying to invoke CLEO opcode `045A`.
- Clarified that CFont setters mutate shared engine state and that no draw hook or font call is implemented yet.
- Resolved transient page value `32`: the VC SDK enum names it `MENUPAGE_PAUSE_MENU`, while MAP remains page `6`.
- Kept the next implementation gate unchanged: validate a safe in-frame callback before producing a fixed MAP-only text overlay.

## 2026-11-09 — Vanilla MAP render path traced

- Confirmed from VC 1.0 disassembly that page value `6` conditionally calls the original map renderer at `0x0049A5B7`.
- Confirmed the surrounding menu routine continues drawing UI after the map renderer.
- Cross-checked the final draw call at `0x00578710` as `CSprite2d::Draw(CRect const&, CRGBA const&)`.
- Identified the routine return at `0x004A37A1` as the next caller/outer-render trace boundary, not yet an approved hook.
- No hook, font call, map replacement, or asset modification was introduced.

## 2026-11-09 — Frontend menu draw callback candidate identified

- Cross-checked Plugin-SDK's VC 1.0 `menuDrawingEvent` call sites (`0x4A5D33`, `0x4A612E`) against the exact supplied executable; both call `0x4A37A4` with the menu manager in ECX.
- Confirmed the during-session callback site at `0x4A612E` is followed by `CFont::DrawFonts` at `0x4A613D`, whose wrapper calls `RenderFontBuffer`.
- Identified a promising after-menu/before-font-flush route for queuing a fixed text label, pending checks of intervening calls and runtime callback behavior.
- No rendering hook or CFont call was installed.

## 2026-11-08 — Runtime probe verified
- Recorded the owner's successful read-only probe tests across two game runs.
- Confirmed runtime menu-manager, MAP-page, mouse-coordinate, and show-mouse values.
- No production drawing hook, map replacement, or asset modification was introduced.

## 2026-11-01 — Project formalization
- Renamed the repository to **Vice-City-Pause-Map-Expansion**.
- Updated README and GitHub Actions naming to match the project.
- Added project status and changelog documentation.
- Preserved the minimal ASI probe; no game hooks or map modifications were introduced.

### Verified before this documentation update
- Windows x86 MSVC workflow completed successfully.
- Binary verified as PE x86 (machine `0x014c`).
- User confirmed the ASI load-test message appeared in GTA Vice City.

### Not yet implemented
- Pause-menu MAP text overlay.
- Mouse-to-map coordinate conversion.
- Hover location names.
- Right-click waypoint placement.
