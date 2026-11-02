# VC 1.0 MAP Page Branch and Draw-Routine Findings

Executable SHA-256: `04e4db72629eaa786fdd182ac224f4fd6d68806f5f4fe5c1fb5f756dc4da11d7`.

## Important page-value cross-check

The menu routine compares the current-page field at `[ESI+0xF8]` against the immediate value `0x22`:

```asm
004A3302  cmp dword ptr [esi+0xF8], 0x22
004A3309  je  0x4A3508
```

This is direct evidence that page value `0x22` receives a special path in this section of the routine. It is a strong candidate for the MAP page value, but the numeric enum still needs to be cross-checked against the exact VC menu-page enum/header or runtime observation before hard-coding it.

**Correction / caution:** an earlier user-supplied CLEO probe used `page == 6`. The executable evidence above does not support treating 6 as the MAP page value. Do not use 6 in the ASI page guard without further proof.

## Mouse fields

The routine reads:

- `[ESI+0x130]` at `0x4A3703`
- `[ESI+0x12C]` at `0x4A3739`

It then uses those values in floating-point coordinate calculations and passes a structure to `0x578710`. This corroborates the candidate mouse Y/X offsets from the earlier probe notes, but still does not establish screen-to-map conversion.

## Why the routine tail is not yet the overlay point

At `0x4A3793`, the routine calls `0x578710`, then cleans up its stack and callee-saved registers and returns at `0x4A37A1`. This is a coherent function tail, but the preceding work is still part of menu geometry/input processing. We have not proven that it runs after the MAP renderer or at a point where the 2D font pipeline is safe.

## Decision

No ASI hook is added from this evidence. The next step is to resolve the MAP page enum from the exact VC SDK/header or a controlled runtime read, then trace the actual original map render call path and identify a post-render callback. Preserve the vanilla map renderer.
