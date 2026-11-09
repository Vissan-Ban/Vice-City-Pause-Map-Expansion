# Project Status

**Project:** Vice City Pause Map Expansion  
**Target:** GTA Vice City PC 1.0, 32-bit  
**Status:** Runtime probe verified; native font API mapped; vanilla MAP draw path traced; hook validation in progress

## Verified
- [x] Windows x86 MSVC build workflow succeeds; output is PE x86 (`0x014c`).
- [x] User confirmed ASI load test in-game.
- [x] Read-only runtime probe tested in two game runs.
- [x] Main module base `0x00400000`, menu manager base `0x00869630`, MAP page `6`, mouse fields, and show-mouse flag confirmed at runtime.
- [x] VC-specific CFont addresses, cdecl signatures, and wide-character print path cross-checked against Plugin-SDK.
- [x] Page value `32` identified as `MENUPAGE_PAUSE_MENU`.
- [x] Disassembly confirms the larger menu routine dispatches to vanilla MAP renderer `0x0049A5B7` only when page field `[ESI+0xF8]` equals `6`, then continues drawing other UI.
- [x] Plugin-SDK identifies the routine's final call at `0x00578710` as `CSprite2d::Draw(CRect const&, CRGBA const&)`.

## Current implementation
`src/runtime_probe.cpp` remains read-only. No drawing hook or CFont call has been added.

## Current next milestone
Trace the caller and outer frontend render order around the routine ending at `0x004A37A1`. Determine whether a safe after-menu draw event exists. Then build a fixed MAP-only text test using the native VC CFont API.

Do not hook or replace `0x0049A5B7`; preserve the vanilla map renderer. Do not draw from the probe worker thread.

## Not implemented / unverified
- Safe post-menu callback.
- Fixed text overlay and runtime CFont validation.
- Exact map rectangle and mouse-to-map conversion.
- Hover location names and right-click waypoint.

## Engineering rules
1. Verify addresses against the exact VC 1.0 executable and runtime.
2. Keep experiments small and reversible.
3. Record build, commit, artifact, and in-game result.
4. Compilation alone does not mean a feature is complete.
5. Preserve vanilla map drawing and assets; no IPL, map-data, or texture replacement.
