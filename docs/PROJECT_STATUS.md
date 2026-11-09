# Project Status

**Project:** Vice City Pause Map Expansion  
**Target:** GTA Vice City PC 1.0, 32-bit  
**Status:** Runtime probe verified; native font API mapped; draw-hook validation

## Verified
- [x] Repository formalized and renamed.
- [x] Windows x86 MSVC build workflow succeeds; output is PE x86 (`0x014c`).
- [x] User confirmed ASI load test in-game.
- [x] Read-only runtime probe tested in two game runs.
- [x] Main module base `0x00400000` and menu manager base `0x00869630` confirmed.
- [x] Page offset `0xF8`, MAP page value `6`, mouse offsets `0x12C/0x130`, and show-mouse offset `0x140` read successfully at runtime.
- [x] Mouse coordinates changed during MAP use; show-mouse flag read `1`; no read errors reported.
- [x] VC-specific CFont function addresses, cdecl signatures, and wide-character print path cross-checked against Plugin-SDK source.
- [x] Transient page value `32` identified by the VC SDK enum as `MENUPAGE_PAUSE_MENU`.

## Current implementation
`src/runtime_probe.cpp` remains a read-only diagnostic probe. It does not install rendering hooks or draw text.

## Next milestone
Identify and validate a safe in-frame callback after vanilla MAP content is drawn. Then build a fixed MAP-only text overlay using the VC native CFont API. Compare its appearance with the familiar CLEO `045A` style.

The probe worker thread is not a rendering callback. Do not draw from it. CFont setters alter shared engine state; do not add drawing until state handling and callback timing are understood.

## Not implemented / unverified
- Fixed text overlay and safe draw callback.
- Runtime validation of CFont calls in our ASI.
- Exact map rectangle and mouse-to-map conversion.
- Hover location names and right-click waypoint.

## Engineering rules
1. Verify addresses against the exact VC 1.0 executable and runtime.
2. Keep experiments small and reversible.
3. Record build, commit, artifact, and in-game result.
4. Compilation alone does not mean a feature is complete.
5. Preserve vanilla map drawing and assets; no IPL, map-data, or texture replacement.
