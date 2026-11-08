# Project Status

**Project:** Vice City Pause Map Expansion  
**Target:** GTA Vice City PC 1.0, 32-bit  
**Status:** Runtime probe verified; draw-hook and font-path research

## Verified

- [x] GitHub repository formally renamed.
- [x] Windows x86 MSVC build workflow completes successfully.
- [x] Baseline and runtime-probe outputs are 32-bit x86 PE files (`0x014c`).
- [x] User confirmed the ASI load-test message appears in-game.
- [x] Runtime probe was manually tested in two game runs.
- [x] Main module base `0x00400000` and menu manager base `0x00869630` were confirmed.
- [x] Current menu page offset `0xF8`, MAP page value `6`, mouse offsets `0x12C/​0x130`, and show-mouse offset `0x140` were read successfully at runtime.
- [x] Mouse coordinates changed during MAP-page use; show-mouse byte read `1`; no read errors were reported.
- [x] The read-only probe did not replace the original map renderer or modify map assets.

See [VC10_RUNTIME_PROBE_RESULTS.md](VC10_RUNTIME_PROBE_RESULTS.md) for the captured test observations.

## Current implementation

`src/runtime_probe.cpp` is a read-only diagnostic probe. It reads menu page, mouse coordinates, and show-mouse state and writes a log beside the game executable (with a temporary-directory fallback). It does not install rendering hooks or draw text.

## Current research direction

The next milestone is a fixed text overlay on the original pause-menu MAP page. Research must establish a safe in-frame callback and the correct VC 1.0 native font calls before implementation.

Font strategy:
1. Preferred: call Vice City's own native CFont routines from ASI.
2. Fallback appearance target: reproduce the familiar CLEO `045A` text style using the same game font renderer, not by attempting to invoke a CLEO opcode from ASI.

The read-only probe's worker thread is not a rendering callback. Do not draw from it.

## Not yet implemented or verified

- Fixed text overlay on MAP.
- Safe post-map-draw hook / callback.
- Exact map rectangle and mouse-to-map coordinate conversion.
- Zone-name hover display.
- Right-click waypoint placement.
- Meaning of transient page value `32` observed during one MAP session.

## Engineering rules

1. Treat addresses and structure offsets as hypotheses until verified against the exact VC 1.0 executable and runtime.
2. Keep each experiment small and reversible.
3. Record build run, commit, artifact, and in-game test outcome for each milestone.
4. Do not describe a feature as complete based only on successful compilation.
5. Preserve vanilla map drawing and assets; no IPL, map-data, or texture replacement.
