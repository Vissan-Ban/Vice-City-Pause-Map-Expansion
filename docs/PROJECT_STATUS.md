# Project Status

**Project:** Vice City Pause Map Expansion  
**Target:** GTA Vice City PC 1.0, 32-bit  
**Status:** Runtime probe verified; CFont API mapped; strongest menu draw callback candidate identified

## Verified
- [x] Runtime probe tested in two game runs; menu manager, MAP page 6, mouse fields, and show-mouse flag confirmed.
- [x] VC-specific CFont addresses and cdecl/wide-string API cross-checked against Plugin-SDK.
- [x] Original MAP renderer identified at `0x0049A5B7`; do not replace it.
- [x] Plugin-SDK VC 1.0 `menuDrawingEvent` call sites cross-checked against the exact executable: `0x004A5D33` and `0x004A612E`, both call `0x004A37A4` with menu manager in ECX.
- [x] During-session site `0x004A612E` occurs before the subsequent `CFont::DrawFonts` call at `0x004A613D`; DrawFonts calls RenderFontBuffer.

## Best current candidate
Use the SDK-style after-call menuDrawingEvent at `0x004A612E`, with a MAP-page guard (6), to queue one fixed test string through CFont. The normal later DrawFonts call may flush it. This ordering is promising but has not yet been tested in-game. Calls at `0x004A7060` and `0x005566E0` between the callback and font flush still need inspection for font-state/buffer side effects.

## Current implementation
`src/runtime_probe.cpp` remains read-only. No rendering hook or CFont call has been added.

## Next milestone
Inspect the two intervening functions, validate event callback semantics and page state, then build a separate fixed-text ASI test. Preserve vanilla MAP drawing and all assets.

## Not implemented / unverified
- Runtime callback frequency and font-buffer behavior.
- Fixed text overlay.
- Map rectangle conversion, hover labels, waypoint.

## Engineering rules
1. Verify against the exact VC 1.0 executable.
2. Keep experiments small and reversible.
3. Record build, commit, artifact, and in-game outcome.
4. Compilation alone is not feature completion.
5. No map renderer replacement, IPL, map-data, or texture replacement.
