# VC 1.0 Menu Text Submission Function — Disassembly Addendum

## Target binary

The project owner's VC 1.0 executable:
- SHA-256: `04e4db72629eaa786fdd182ac224f4fd6d68806f5f4fe5c1fb5f756dc4da11d7`
- Image base: `0x00400000`

## Verified call sequence

At the during-session frontend path:

```asm
004A612E  call 0x004A37A4
004A6133  call 0x004A7060
004A6138  call 0x005566E0
004A613D  call 0x00550250
```

The call at `0x4A613D` is `CFont::DrawFonts()`. Thus the interval after the return from `0x5566E0` and before `0x550250` is the last known insertion window before the normal font-buffer flush.

## Correction: what 0x4A7060 and 0x5566E0 actually do

Disassembly of `0x4A7060` shows frontend/menu timing and state work, including calls into menu/UI objects. The inspected range contains no direct CFont calls. It should not be described as merely a no-op or as proven to leave all render state untouched.

Disassembly of `0x5566E0` proves that it is a substantial frontend text-and-UI submission routine. It iterates records rooted at `0x007F0EA0` in 0xF4-byte steps (48 entries), and for eligible entries calls CFont setters including:
- `0x550230` SetScale
- `0x5500F0` SetSlant (per SDK mapping)
- `0x5500D0` SetBackgroundOff
- `0x550E0` / `0x550E0`-adjacent state calls (exact labels require SDK cross-check)
- `0x550A0` and `0x55080` text alignment/justification state calls (exact labels require SDK cross-check)
- `0x54FFE0` SetFontStyle
- `0x54FF20` SetDropShadowPosition
- `0x54FF30` SetDropColor
- `0x550170` SetColor
- `0x551040` PrintString

It also processes additional sprite/rectangle entries and other conditional frontend text after the first 48-entry loop. Therefore the earlier claim that it was just a harmless state step was incorrect. It actively submits vanilla frontend text and mutates shared CFont state.

## Hook-window assessment

The strongest remaining candidate is the instruction boundary immediately after the call at `0x4A6138` returns and before `0x4A613D` calls DrawFonts. At this point vanilla frontend text submission has completed for this pass, and the normal font flush has not yet run.

This is a **candidate only**, not yet an approved implementation. A test hook must still establish:
1. Exact expected bytes at the call site and safe patch length.
2. Whether a call-site trampoline can preserve all live general registers, flags, stack alignment, and x87 state.
3. Whether the page field at `0x869728` is reliably 6 at this point when the MAP page is active.
4. Which CFont state setters are needed for a visibly distinguishable test, and how to avoid leaving global font state altered for later UI.
5. Whether queued text survives to `DrawFonts` and is visible in-game.

Do not call CFont from the runtime-probe worker thread. Do not alter the vanilla map renderer at `0x49A5B7`.

## Current implementation status

No rendering hook or MAP TEST ASI has been installed or built. The existing runtime probe remains the only verified ASI artifact. Keep its workflow job intact when adding a separate test target.
