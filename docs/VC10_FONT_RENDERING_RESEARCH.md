# VC 1.0 Native Font Rendering Research

## Goal

Draw one fixed diagnostic label over the original pause-menu MAP page without replacing the map renderer or changing game assets. The preferred font path is Vice City's own font renderer. The visual fallback is to reproduce the familiar CLEO 045A-style result through that same native renderer.

## Important distinction: CLEO opcode vs game function

045A is a CLEO script opcode, not a Windows API and not a standalone font file. An ASI does not need to invoke the CLEO opcode. The ASI can call the game's native font functions directly, provided the exact VC 1.0 function addresses, calling conventions, state requirements, and safe render timing are verified.

The intended approach is therefore:
1. Identify VC 1.0's native CFont functions and their exact signatures.
2. Identify the font state that affects the desired 045A-like appearance (style, scale, color, alignment, outline/shadow, proportional mode, etc.).
3. Call the native renderer only from a verified in-frame callback, not from the read-only probe's worker thread.
4. Restore any global font state changed by the overlay, where the native API permits it.
5. First display a fixed ASCII test string on MAP only; do not yet add dynamic district labels.

## External SDK reference

DK22Pac's Plugin-SDK supports GTA Vice City ASI plugins and exposes the game's native APIs. Its documentation recommends using CFont::PrintString for text rather than script-draw variables for general UI drawing. This supports the feasibility of a native font path, but does not by itself validate a particular function address, signature, or hook location for this project's exact VC 1.0 executable.

References:
- https://github.com/DK22Pac/plugin-sdk
- https://github.com/DK22Pac/plugin-sdk/blob/master/plugin_vc/game_vc/CFont.h
- https://github.com/DK22Pac/plugin-sdk/blob/master/plugin_vc/game_vc/CFont.cpp

## Current project evidence

Runtime-probe tests confirmed the live VC 1.0 MAP page value (6), menu manager address (0x00869630), mouse fields, and show-mouse flag. See [VC10_RUNTIME_PROBE_RESULTS.md](VC10_RUNTIME_PROBE_RESULTS.md).

The existing draw research identified menu drawing call sites at 0x004A325E and 0x004A32AD, but 0x004A32AD returns into a longer caller sequence and is not proven to be the final overlay point. See [VC10_MENU_DRAW_RESEARCH.md](VC10_MENU_DRAW_RESEARCH.md).

## Open questions before implementation

- Exact VC 1.0 addresses and calling conventions for all required CFont functions.
- Whether the SDK signatures match the supplied executable build.
- Which render callback runs after vanilla MAP content and before frame presentation.
- Whether the callback is called once per frame and whether menu page state is stable there.
- How to preserve/restore font state so the overlay cannot contaminate later menu text.
- Exact coordinate scaling behavior across resolutions and aspect ratios.

## Acceptance test

A future test ASI must:
- draw a fixed label only when current page is 6;
- leave the original map renderer untouched;
- avoid drawing from the diagnostic worker thread;
- leave other menu pages visually unchanged;
- survive repeated MAP open/close cycles without crash or persistent font-state changes.

No font-rendering hook or overlay is implemented by this research note.