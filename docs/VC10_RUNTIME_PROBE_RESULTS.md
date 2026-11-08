# VC 1.0 Runtime Probe Results

## Test scope

The project owner manually tested the read-only runtime probe ASI in GTA Vice City PC 1.0, entered the original pause-menu MAP page, moved the mouse across the map, left the page, and repeated the test. Two separate game runs were captured in the same log file.

The probe reads menu state and mouse fields only. It does not write game memory, install a drawing hook, replace the map renderer, or modify game assets.

## Confirmed runtime values

| Item | Observed value |
|---|---:|
| Main module base | `0x00400000` |
| FrontEndMenuManager base | `0x00869630` |
| Current page offset | `0xF8` |
| MAP page value | `6` |
| Mouse X offset | `0x12C` |
| Mouse Y offset | `0x130` |
| Show-mouse byte offset | `0x140` |
| Show-mouse value during logged MAP samples | `1` |

Absolute addresses derived from the confirmed base and offsets:

- Current page: `0x00869728`
- Mouse X: `0x0086975C`
- Mouse Y: `0x00869760`
- Show mouse: `0x00869770`

These match the VC-specific menu-manager layout research and were also read successfully at runtime.

## Observed behavior

- Both runs reached page value `6` and logged mouse samples while on MAP.
- Mouse coordinates changed as the user moved the pointer.
- The logged coordinates included values near the left/top and right/bottom portions of the screen, including `(0, 619)`, `(1553, 397)`, `(1049, 1079)`, and `(960, 540)`.
- The show-mouse byte was `1` in the MAP samples.
- No `READ ERROR` was reported.
- Run 2 briefly logged transitions `6 -> 32 -> 6` while the user remained in the MAP workflow. The meaning of page value `32` is not established; do not assume it is a draw phase or a particular input state.
- Leaving MAP eventually produced transitions away from page `6`.

## What this proves

The probe successfully reads the expected VC 1.0 menu-manager fields during actual gameplay. The MAP page value and live mouse coordinates are now runtime-confirmed, not merely inferred from SDK declarations.

## What this does not prove

- The logged mouse values are not yet converted into map-world coordinates.
- The actual visible map rectangle and its scaling/letterboxing behavior are not yet established.
- The meaning of transient page value `32` is unknown.
- No text rendering call or draw-hook timing has been tested.
- Successful memory reads do not prove that rendering from the probe's worker thread is safe. Rendering must be investigated separately and should execute on a verified game/render thread.

## Next milestone

Inspect the VC 1.0 native font API and identify a safe in-frame callback after the vanilla pause-map content is drawn. First render a fixed test label on MAP only. Compare the native font appearance with the familiar CLEO `045A` text style. Do not implement hover labels or waypoint behavior until this fixed overlay is stable.
