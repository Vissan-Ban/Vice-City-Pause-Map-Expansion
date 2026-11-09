# VC 1.0 Menu Page Runtime Interpretation

The runtime probe recorded brief transitions from page value `6` to `32` and back while the user was navigating the pause menu.

The VC-specific Plugin-SDK enum `eMenuScreen` defines:

- `MENUPAGE_MAP = 6`
- `MENUPAGE_PAUSE_MENU = 32`

Therefore the observed `6 -> 32 -> 6` sequence is consistent with a transition from the MAP page to the pause-menu page and back. It is not evidence of a special drawing phase or input-processing state.

This interpretation is supported by the VC SDK enum and matches the runtime values observed by the read-only probe. The exact UI action responsible for each brief transition is not inferred from the log alone.

Reference: [VC CMenuManager.h](https://github.com/DK22Pac/plugin-sdk/blob/master/plugin_vc/game_vc/CMenuManager.h).
