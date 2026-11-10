# MenuMap 0.120: Native Font Submission Findings

## Why this reference matters

The supplied MenuMap 0.120 source demonstrates a working Vice City native-font submission path inside its custom pause-map draw routine. It does not implement its own glyph renderer.

Relevant source: `source/MenuNew.cpp`, `CMenuNew::DrawZone()`.

For Vice City, the code configures the native CFont state and submits the translated zone label with `CFont::PrintString()`:

```cpp
CFont::SetPropOn();
CFont::SetBackgroundOff();
CFont::SetCentreOff();
CFont::SetWrapx(SCREEN_WIDTH);
CFont::SetRightJustifyWrap(0.0f);
CFont::SetColor(CRGBA(r, g, b, alpha));
CFont::SetDropColor(CRGBA(0, 0, 0, 255));
CFont::SetDropShadowPosition(2);
CFont::SetFontStyle(2);
CFont::SetScale(ScaleX(0.64f), ScaleY(1.28f));
CFont::SetRightJustifyOn();
CFont::PrintString(x, y, str);
```

This is the GTAVC branch of `DrawZone()` (not the GTA3 or GTASA variants). The source then continues with other map drawing work. The important reusable lesson is that the plugin submits text through the game's CFont API; it does not draw letters through Direct3D itself.

## Render-order implication

MenuMap 0.120 replaces the original map renderer at `0x49A5B7`, and calls `DrawZone()` from its replacement `DrawMap()`. Our previously traced VC 1.0 menu sequence is:

1. `0x4A612E`: menu drawing call / plugin-sdk menu drawing event site
2. `0x4A6133`: frontend text/UI helper at `0x5566E0`
3. `0x4A613D`: original call to `CFont::DrawFonts` at `0x550250`

Thus MenuMap's successful pattern strongly suggests that calling `CFont::PrintString()` while the MAP renderer is executing can enqueue text that the later original `DrawFonts()` call renders. This is the most useful clue for our non-invasive implementation: submit text during the original MAP renderer's execution, but leave the renderer and the later `DrawFonts()` call intact.

This is a source-backed inference from the plugin's call path and the known vanilla call sequence. It is not yet an in-game proof that our exact hook placement and font state are correct.

## First test implementation

- Keep the original function at `0x49A5B7`; do not redirect or replace it.
- Add a narrowly scoped hook around the original renderer call, or another verified point inside its execution window, that submits a fixed ASCII test label using the same VC CFont setup.
- Return to the original flow untouched.
- Let the existing call at `0x4A613D` invoke the game's original `CFont::DrawFonts()`.
- Do not call `DrawFonts()` a second time.
- Gate submission on `m_nCurrentMenuPage == 6` and the verified VC 1.0 executable identity.
- Preserve the Runtime Probe target and artifact as a separate build target.

## Safety / open verification items

- Confirm the CFont wrapper addresses and calling conventions against the exact Plugin-SDK version used by the project before writing the hook.
- Confirm the hook preserves all registers/flags and the original renderer executes exactly once.
- Confirm font settings are reset or deliberately set for every submission; do not depend on state left by the previous frame.
- Start with a fixed label and fixed screen position. No mouse mapping, zone lookup, or waypoint logic belongs in this first test.
- Do not patch `0x49A5B7` with a replacement map renderer. Do not patch or skip `0x4A613D`.

## Conclusion

MenuMap 0.120 gives us a concrete, native CFont recipe and a strong render-order clue. It narrows the first test to a small task: submit one fixed string while the original MAP renderer is running and allow the original frontend `DrawFonts()` call to render it.
