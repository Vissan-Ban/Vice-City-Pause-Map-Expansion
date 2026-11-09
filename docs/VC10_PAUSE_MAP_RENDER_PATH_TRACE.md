# VC 1.0 Pause MAP Render-Path Trace — 2026-11-09

## Executable

The trace below was produced from the project owner's supplied VC 1.0 executable:

- PE32 i386, ImageBase `0x00400000`
- SHA-256: `04e4db72629eaa786fdd182ac224f4fd6d68806f5f4fe5c1fb5f756dc4da11d7`

## Important new finding: the vanilla map renderer is called inside a larger menu draw routine

At `0x004A2CB7`, the routine compares the current page field:

```asm
004A2CB7  cmp dword ptr [esi+0xF8], 6
004A2CBE  jne 0x004A2CC7
004A2CC0  mov ecx, esi
004A2CC2  call 0x0049A5B7
```

This is strong instruction-level confirmation that the page value `6` dispatches to the vanilla MAP renderer at `0x0049A5B7`. The renderer is not the whole menu draw routine: execution continues at `0x004A2CC7` and performs further UI drawing.

This distinction matters for the project: hooking the map renderer itself risks drawing before later vanilla menu elements. A fixed overlay should be drawn after the original menu draw sequence, not by replacing `0x0049A5B7`.

## What happens near the end of the larger routine

The path near the end includes:

```asm
004A3793  call 0x00578710
004A3798  add  esp, 0xD8
004A379E  pop  ebp
004A379F  pop  esi
004A37A0  pop  ebx
004A37A1  ret  4
```

Plugin-SDK identifies `0x00578710` as:

`CSprite2d::Draw(CRect const&, CRGBA const&)`

This call is therefore a sprite/rectangle draw, not a CFont call. It is part of the menu routine's final visible UI work (the exact semantic role of this particular rectangle is not assigned here).

The routine returns at `0x004A37A1`. That makes the return boundary a more meaningful place to investigate than the earlier `0x004A32AD` call site, but it is **not yet an approved hook**: the caller, call frequency, outer render order, and whether later code redraws over the screen still need to be established.

## Consequences for hook selection

1. Do not replace or detour `0x0049A5B7`; it is the original map renderer and must remain intact.
2. Do not assume `0x004A32AD` is post-render. It returns into additional CFont-buffer and menu drawing work.
3. Do not call CFont from the read-only probe worker thread.
4. Continue upward from the `0x004A37A1` return to identify the caller and the larger frontend/render sequence. Determine whether a stable RenderWare/plugin-sdk draw event occurs after this routine and before presentation.
5. If no suitable general draw event can be proven, investigate a minimal return-site trampoline around this specific menu routine, with a runtime guard for page `6`, and verify register/stack preservation and repeat-call behavior before enabling any text.

## Current confidence

- **Confirmed by executable disassembly:** page value `6` conditionally calls `0x0049A5B7`; execution continues afterward; the routine ends at `0x004A37A1`.
- **Confirmed by Plugin-SDK mapping:** `0x00578710` is `CSprite2d::Draw(CRect const&, CRGBA const&)`.
- **Not yet confirmed:** the enclosing routine's formal identity, its callers, whether its return is the last relevant frontend draw before frame presentation, or whether a hook there is safe.
- **No code hook or font call has been installed.**

## Next trace target

Identify the routine entry and all direct callers of the routine ending at `0x004A37A1`; then map the parent frontend rendering sequence and compare it with available VC RenderWare/plugin-sdk events. Only after this should a fixed MAP-only text overlay be implemented.
