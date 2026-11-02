# VC 1.0 Menu Manager SDK Cross-Check

**Target executable:** GTA Vice City PC 1.0 (32-bit)  
**Reference:** DK22Pac/plugin-sdk, `plugin_vc/game_vc/CMenuManager.h` and `CMenuManager.cpp`  
**Checked:** 2026-11-02

## Confirmed menu page enum

The VC-specific Plugin-SDK header defines:

```cpp
enum eMenuScreen {
    MENUPAGE_NONE = -1,
    MENUPAGE_STATS = 0,
    MENUPAGE_NEW_GAME = 1,
    MENUPAGE_BRIEFS = 2,
    MENUPAGE_SOUND_SETTINGS = 3,
    MENUPAGE_DISPLAY_SETTINGS = 4,
    MENUPAGE_LANGUAGE_SETTINGS = 5,
    MENUPAGE_MAP = 6,
    // ...
};
```

Therefore **`MENUPAGE_MAP == 6` is confirmed by the VC-specific SDK**, not merely guessed from the earlier CLEO probe. This resolves the previously open page-enum question at the SDK-definition level. Runtime observation on the user's exact installation remains a useful final check.

## Confirmed menu manager address and field offsets

The VC SDK implementation defines:

```cpp
CMenuManager& FrontEndMenuManager = *(CMenuManager *)0x869630;
```

The same header validates these offsets:

| Field | Offset |
|---|---:|
| `m_nCurrentMenuPage` | `0xF8` |
| `m_nMousePosX` | `0x12C` |
| `m_nMousePosY` | `0x130` |
| `m_nMouseOldPosX` | `0x134` |
| `m_nMouseOldPosY` | `0x138` |
| `m_bShowMouse` | `0x140` |
| `sizeof(CMenuManager)` | `0x684` |

Combined with the SDK base address, the corresponding absolute addresses are:

| Field | Absolute address |
|---|---:|
| Current menu page | `0x869728` |
| Mouse X | `0x86975C` |
| Mouse Y | `0x869760` |
| Old mouse X | `0x869764` |
| Old mouse Y | `0x869768` |
| Show mouse | `0x869770` |

These addresses are derived from the SDK's declared object address plus its validated member offsets. They are not yet a substitute for a runtime read test against the user's exact executable/mod environment.

## Reconciliation with our executable analysis

Our earlier disassembly observed reads from `[ESI+0x12C]` and `[ESI+0x130]`, and a comparison of `[ESI+0xF8]`. The SDK now independently identifies those fields as mouse X, mouse Y, and current menu page.

This is a strong cross-check that the disassembled object is the VC menu manager. The SDK's page enum also confirms that value `6` is MAP; value `0x22` seen in a different branch comparison is not the MAP page enum.

## Important limitation

This resolves the menu-page value, manager address, and relevant member offsets on the SDK-definition level. It does **not** identify a safe overlay callback, prove the best drawing phase, or implement any map feature.

No hooks or memory patches are introduced by this research note. The next implementation step remains a conservative read-only runtime probe, followed by a fixed text overlay once a safe draw point is established.

## Sources

- [Plugin-SDK VC CMenuManager.h](https://github.com/DK22Pac/plugin-sdk/blob/master/plugin_vc/game_vc/CMenuManager.h)
- [Plugin-SDK VC CMenuManager.cpp](https://github.com/DK22Pac/plugin-sdk/blob/master/plugin_vc/game_vc/CMenuManager.cpp)
