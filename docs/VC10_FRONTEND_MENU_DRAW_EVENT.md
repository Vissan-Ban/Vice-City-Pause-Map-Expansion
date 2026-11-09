# VC 1.0 Frontend Menu Draw Event — Caller Trace

## Exact executable
Project-owner supplied VC 1.0 PE32 executable, ImageBase `0x00400000`, SHA-256 `04e4db72629eaa786fdd182ac224f4fd6d68806f5f4fe5c1fb5f756dc4da11d7`.

## Plugin-SDK event cross-check

DK22Pac/plugin-sdk `shared/Events.h` defines the VC 1.0 event:

```cpp
static inline ThiscallEvent<
    AddressListMulti<
        0x4A5D33, GAME_10EN, H_CALL, // before game session
        0x4A612E, GAME_10EN, H_CALL  // during game session
    >,
    PRIORITY_AFTER,
    ArgPickNone,
    void(CMenuManager*)
> menuDrawingEvent;
```

The exact supplied executable confirms both call sites target `0x4A37A4`:

- At `0x4A5D33`: `mov ecx, 0x869630; call 0x4A37A4`.
- At `0x4A612E`: `mov ecx, 0x869630; call 0x4A37A4`.

The target function begins by preserving EBX and copying ECX into EBX, consistent with a thiscall method receiving the menu manager in ECX. Its return is `ret 4` at `0x4A37A1`; the SDK's after-call event is attached to the caller's CALL instruction, not to an arbitrary guessed epilogue.

## Critical during-session sequence

At the during-session call site:

```asm
004A6120  cmp byte ptr [0x869668], 0
004A6127  je  0x004A6133
004A6129  mov ecx, 0x00869630
004A612E  call 0x004A37A4
004A6133  call 0x004A7060
004A6138  call 0x005566E0
004A613D  call 0x00550250
004A6142  call 0x00544797
004A6147  call 0x00544797
...
004A617C  add esp, 0x20
004A6183  ret
```

Plugin-SDK identifies `0x550250` as `CFont::DrawFonts()`; its implementation immediately calls `0x551A30` (`CFont::RenderFontBuffer()`). This is a significant ordering clue: an after-call callback at the `0x4A612E` menuDrawingEvent site occurs before the subsequent `CFont::DrawFonts` flush.

Therefore a callback that queues text using `CFont::PrintString` after the menu draw function and before the later DrawFonts call is a plausible route: the normal engine font flush may render the queued string. This is a **candidate mechanism**, not yet runtime-proven. Calls at `0x4A7060` and `0x5566E0` occur between the event and DrawFonts; their effects on font state/buffer must be checked, and the exact page guard must be verified at callback time.

## Earlier/menu-before-session call site

At `0x4A5D33`, the same `CMenuManager::Draw`-like routine is called before the game session. The following sequence differs and should not be used as the first overlay path. The during-session site `0x4A612E` is the relevant candidate for pause-menu runtime.

## Other event candidate: drawingEvent

Plugin-SDK also defines a VC 1.0 `drawingEvent` at `0x4A61D6`. The exact executable shows this is a call to `0x620BE0` later in the same outer routine, after the menu drawing and font flush. It may be a later frame-stage callback, but its function identity and whether a 2D CFont draw is still valid there are not established. It is not selected yet.

## Current hook decision

**Best candidate to prototype:** Plugin-SDK's VC 1.0 `menuDrawingEvent` after-call hook at `0x4A612E`, guarded by `FrontEndMenuManager.m_nCurrentMenuPage == MENUPAGE_MAP` (page 6), queueing one fixed text string with CFont before the subsequent `CFont::DrawFonts` call.

Before implementation, verify:
1. What `0x4A7060` and `0x5566E0` do to CFont state and queued text.
2. Whether the menu page remains 6 at the after-call callback for the map.
3. Whether the plugin event's trampoline preserves the caller's stack/register state as expected.
4. Whether `PrintString` at that point queues text that the subsequent `DrawFonts` actually renders.
5. Whether this site is called once per rendered frame while the MAP page is open, and whether the callback is skipped cleanly outside gameplay.

Do not hook `0x49A5B7` (vanilla map renderer) and do not draw from the diagnostic worker thread. No hook or CFont call has yet been installed.
