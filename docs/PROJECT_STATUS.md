# Project Status

**Project:** Vice City Pause Map Expansion  
**Target:** GTA Vice City PC 1.0, 32-bit  
**Status:** Fixed CFont text overlay visually verified; lower-right placement iteration pending owner confirmation

## Verified
- [x] Runtime probe tested in-game; menu manager, MAP page 6, mouse fields, and show-mouse flag confirmed.
- [x] VC-specific CFont addresses and cdecl/wide-string API cross-checked against Plugin-SDK.
- [x] Original MAP renderer identified at `0x0049A5B7`; it is not replaced by the fixed-text hook.
- [x] Fixed text successfully displayed by the owner on the original MAP page using a separate ASI artifact.
- [x] The hook site `0x004A613D` and original five-byte call signature were validated against the supplied VC 1.0 executable.
- [x] The hook queues text before the original `CFont::DrawFonts` call, which is still invoked exactly once by the thunk.

## Current positioning experiment
The first owner-verified build displayed `MAP TEXT TEST` at the upper-left. The current iteration moves the text origin to approximately `(420, 360)` in the 640x448 frontend coordinate space, intending to place it in the lower-right while retaining a margin from the bottom native menu text and screen edge.

**Not yet verified:** exact visual alignment, bottom-row clearance, and behavior at other display resolutions/aspect ratios. The owner must test this build in-game before the position is treated as accepted.

## Current implementation
- `src/runtime_probe.cpp`: read-only runtime diagnostic probe; not required for the text overlay.
- `src/map_text_test.cpp`: experimental MAP-only fixed text overlay.
- GitHub Actions builds both artifacts independently as 32-bit x86 ASI files.

## Next milestones
1. Verify the lower-right placement and adjust margins if needed.
2. Inspect the exact map viewport and determine the transformation from mouse screen coordinates to map-relative coordinates.
3. Determine the corresponding world/map coordinate mapping and identify the game's original zone-name lookup path.
4. Add hover location labels only after coordinate mapping is validated.
5. Revisit waypoint placement later; currently deferred.

## Not implemented / unverified
- Mouse-to-map/world coordinate conversion.
- Hover location names.
- Right-click waypoint placement.
- Final release packaging and broad compatibility testing.

## Engineering rules
1. Verify against the exact VC 1.0 executable.
2. Keep experiments small, independently buildable, and reversible.
3. Record commit, artifact, build result, and owner in-game outcome separately.
4. Compilation alone is not feature completion.
5. Preserve the original MAP renderer, IPL/map data, textures, and vanilla assets.
