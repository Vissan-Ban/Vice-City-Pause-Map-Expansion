# VC 1.0 Menu Trace — Page-State and Pointer Offsets

Executable SHA-256: `04e4db72629eaa786fdd182ac224f4fd6d68806f5f4fe5c1fb5f756dc4da11d7`.

## New instruction-level confirmation

In the routine containing the menu-page draw calls, the instruction sequence at `0x004A3201` is:

```asm
004A3201  mov eax, [esi+0xFC]
004A3207  pop ecx
004A3208  mov [esi+0xF8], eax
...
004A3220  mov eax, [esi+0xF8]
004A3226  sub eax, 0x15
004A3229  mov ecx, [esi+0xF8]
```

The code uses the object in `ESI` and reads/writes fields at offsets `0xF8` and `0xFC` as part of menu-page selection. This is consistent with the supplied community probe's hypothesis that `0x869728` (manager base `0x869630` + `0xF8`) holds the current page value, but this observation alone does not prove every runtime object/layout assumption or that a new hook may safely use the field.

The same routine later reads `[esi+0x130]` and `[esi+0x12C]` near `0x4A3703` and `0x4A3739`, consistent with the candidate mouse Y/X fields from the community probe. This is corroborating evidence for field offsets, not a substitute for in-game validation.

## Overlay decision

Do not yet hook the function epilogue at `0x4A3798`: the function is doing coordinate/UI work and calls `0x578710` immediately before its epilogue. We have not established that this routine is the final map render stage or that calling text APIs there is render-state-safe.

The third-party plugin's after-call hooks at `0x4A325E` and `0x4A32AD` are useful for page/input processing, but its VC implementation separately replaces the original map renderer at `0x49A5B7`. We must not copy that replacement.

## Next practical route

Inspect other vanilla frontend draw calls / RenderWare events and the exact VC font drawing entry points, then select a post-map callback whose render state and menu page guard can be verified. The first map-test ASI should only be produced after that call path and calling convention are justified. No executable patch or game data has been changed by this research.
