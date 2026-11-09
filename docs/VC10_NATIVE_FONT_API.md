# VC 1.0 Native Font API Research

## Reference and target

Reference source: DK22Pac/plugin-sdk, VC-specific files:

- `plugin_vc/game_vc/CFont.h`
- `plugin_vc/game_vc/CFont.cpp`

Target executable remains the project owner's supplied GTA Vice City PC 1.0 PE32 executable (ImageBase `0x00400000`, SHA-256 recorded in the menu-draw research notes).

## Confirmed VC-specific function addresses

The VC implementation explicitly maps these CFont methods:

| Method | Address | Signature shown by SDK |
|---|---:|---|
| `SetDropShadowPosition` | `0x54FF20` | cdecl, short |
| `SetDropColor` | `0x54FF30` | cdecl, CRGBA by pointer internally |
| `SetAlphaFade` | `0x54FFC0` | cdecl, float |
| `SetFontStyle` | `0x54FFE0` | cdecl, short |
| `SetPropOn` | `0x550020` | cdecl, no arguments |
| `SetPropOff` | `0x550030` | cdecl, no arguments |
| `SetBackgroundOff` | `0x5500D0` | cdecl, no arguments |
| `SetCentreOff` | `0x550110` | cdecl, no arguments |
| `SetCentreOn` | `0x550120` | cdecl, no arguments |
| `SetJustifyOff` | `0x550140` | cdecl, no arguments |
| `SetJustifyOn` | `0x550150` | cdecl, no arguments |
| `SetColor` | `0x550170` | cdecl, CRGBA by pointer internally |
| `SetScale` | `0x550230` | cdecl, two floats |
| `DrawFonts` | `0x550250` | cdecl, no arguments |
| `GetTextRect` | `0x550720` | cdecl, rect pointer, x, y, wide string |
| `PrintString` | `0x551040` | cdecl, x, y, const wchar_t* |
| `RenderFontBuffer` | `0x551A30` | cdecl, no arguments |
| `InitPerFrame` | `0x5522B0` | cdecl, no arguments |
| `AsciiToUnicode` | `0x552500` | cdecl, const char*, wchar_t* |

The SDK also provides a `CFont::PrintString(float, float, const char*)` convenience overload. Its implementation converts ASCII to Unicode using the game's own `AsciiToUnicode` routine, then calls the wide-character overload.

## Important distinctions

- These are **Vice City** addresses, not GTA III or San Andreas addresses.
- The SDK explicitly identifies the functions as cdecl.
- The native print path expects UTF-16/wide text. For a fixed ASCII test label, the SDK convenience overload or explicit `AsciiToUnicode` conversion is available.
- `DrawFonts`, `RenderFontBuffer`, and `InitPerFrame` are engine pipeline functions. We must not call them speculatively; first identify where the vanilla menu invokes them.
- CFont setters mutate shared engine font state. Before using them in a hook, determine which state the surrounding menu code expects and how to avoid leaking altered alignment, scale, color, shadow, or style into subsequent vanilla text.

## Relevance to CLEO 045A fallback

ASI does not need to execute CLEO opcode `045A`. The fallback is to reproduce the same visible style by calling the VC CFont API directly. The opcode's precise behavior and defaults still need to be checked against the actual VC CLEO implementation before claiming a pixel-identical match.

## Current decision

The native font API is sufficiently identified to prepare a controlled test, but this is not yet a safe draw point. No font call has been added to the ASI and no runtime text rendering has been claimed.

Next: finish identifying a safe in-frame callback after vanilla MAP drawing, then build a fixed text-only test artifact.
