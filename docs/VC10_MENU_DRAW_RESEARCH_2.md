# VC 1.0 Menu Draw Trace — Follow-up

Executable: supplied `gta-vc.exe`, SHA-256
`04e4db72629eaa786fdd182ac224f4fd6d68806f5f4fe5c1fb5f756dc4da11d7`.

Disassembly was performed with LLVM objdump against the PE32 i386 executable.

## Verified control-flow details

The caller containing the two calls to `0x49DF40` continues beyond `0x4A32AD`. The region from `0x4A32B2` onward contains further calls and rendering/state setup; the caller's epilogue is at:

```asm
004A3793  call 0x00578710
004A3798  add  esp, 0xD8
004A379E  pop  ebp
004A379F  pop  esi
004A37A0  pop  ebx
004A37A1  ret  4
```

This gives a useful boundary for understanding the caller's lifetime: `0x4A3798` is close to the end of this routine, but is **not yet approved as an overlay hook**. The routine has not yet been fully identified by symbol/prototype, and the active menu-page state, preserved registers, stack discipline, render-state expectations, and caller relationship still need validation.

## What this does and does not establish

- It establishes that `0x4A32AD` is not the end of the caller.
- It establishes the end of the observed routine at `0x4A37A1`.
- It does not establish that a text draw inserted at `0x4A3798` will be visible, safe, or limited to the MAP page.
- It does not establish that `ESI` at that point can safely be treated as a menu-manager pointer by an injected callback.
- No hook, executable patch, or game-data change is made in this research commit.

## Next validation required

1. Identify the routine entry and all callers of this menu-draw routine.
2. Verify the object held in `ESI` throughout the routine and confirm the menu page offset from the exact executable.
3. Check whether the call at `0x00578710` and the preceding rendering calls leave a valid 2D text-rendering state.
4. Prefer a stable RenderWare draw event after this menu routine if one exists, with an explicit MAP-page guard.
5. Only then implement the fixed `MAP TEST` overlay. Preserve the vanilla map renderer and all original assets.
