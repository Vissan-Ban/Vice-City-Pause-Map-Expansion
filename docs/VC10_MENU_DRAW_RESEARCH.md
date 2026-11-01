# VC 1.0 Menu Draw Research — 2026-11-01

## Executable examined

- File: `gta-vc.exe` supplied by the project owner
- PE: PE32, i386, ImageBase `0x00400000`
- SHA-256: `04e4db72629eaa786fdd182ac224f4fd6d68806f5f4fe5c1fb5f756dc4da11d7`

## Verified caller instructions

At `0x004A325E`:

```asm
004A325A  mov ecx, esi
004A325C  push 0
004A325E  call 0x0049DF40
```

At `0x004A32AD`:

```asm
004A32A9  mov ecx, esi
004A32AB  push 1
004A32AD  call 0x0049DF40
```

Both calls enter the same menu-page drawing routine with the menu manager in ECX and a different stack argument.

## Important correction to the hook hypothesis

The second call is not the end of the surrounding menu draw routine. Its return address is `0x004A32B2`, after which the caller continues with additional drawing/state calls, including calls at `0x004A32B2`, `0x004A32BB`, `0x004A32C6`, `0x004A32D1`, `0x004A32DC`, `0x004A32E7`, and `0x004A32F2`, followed by further menu rendering.

Therefore `0x004A32AD` must not be treated as a proven final overlay point.

## Third-party menu-map source review

The supplied menu-map 0.120 source registers an after-call event at `0x4A325E` and `0x4A32AD` for page/input handling. Its VC map replacement separately redirects `0x0049A5B7` to its own `DrawMap()` implementation. That replacement suppresses the vanilla map draw path and is not acceptable for this project's preservation requirement.

The plugin's custom `DrawMap()` also draws a full-screen background and radar tiles before blips, crosshair, zone label, and legend. It is useful as a reference for later APIs, but must not be transplanted as the first overlay implementation.

## Current conclusion

- The two menu-page call sites and their arguments are verified against the supplied executable.
- The second call returns into a longer caller sequence; it is not a confirmed post-menu-render callback.
- The third-party `0x0049A5B7` replacement is explicitly rejected for this project.
- No production hook or test ASI has been added from this research alone.

## Next investigation

Trace the complete caller through its epilogue and identify a RenderWare/UI draw callback that occurs after the vanilla map content but before the frame is presented. Confirm page-state access and calling convention before adding a fixed `MAP TEST` overlay. The first implementation must leave the vanilla map draw routine and all map assets untouched.
