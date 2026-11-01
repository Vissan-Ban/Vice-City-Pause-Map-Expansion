# Project Status

**Project:** Vice City Pause Map Expansion  
**Target:** GTA Vice City PC 1.0, 32-bit  
**Status:** Foundation / load-probe stage

## Verified

- [x] GitHub repository formally renamed.
- [x] Windows x86 MSVC build workflow completes successfully (baseline run).
- [x] Baseline output is a 32-bit x86 PE (`0x014c`).
- [x] User confirmed the ASI load-test message appears in-game.
- [x] Original game map assets and map rendering have not been modified by the probe.

## Current implementation

`src/probe.cpp` is a minimal visible-load test. It waits briefly and displays a confirmation message. It does not inspect menu state, access game memory, install hooks, or patch executable code.

## Next milestone

Implement a **fixed text overlay on the original pause-menu MAP page**. Before implementation, identify and validate a safe post-map-draw extension point. Do not replace the original map drawing routine.

## Acceptance criteria for the next milestone

- Text appears only while the original MAP page is active.
- Text is drawn over the original map without replacing or suppressing it.
- Other pause-menu pages remain unchanged.
- No texture, IPL, map-data, or unrelated game files are modified.
- The build is x86 and the in-game test is repeatable.

## Engineering rules

1. Treat addresses and structure offsets as hypotheses until verified against the exact VC 1.0 executable.
2. Keep each experiment small and reversible.
3. Record build run, commit, artifact, and in-game test outcome for each milestone.
4. Do not describe a feature as complete based only on successful compilation.
