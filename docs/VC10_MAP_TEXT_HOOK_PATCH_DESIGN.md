# VC 1.0 MAP Text Hook — Exact Call-Site Bytes and Patch Design

## Binary identity

The executable rechecked for this note is PE32 i386, ImageBase `0x00400000`, SHA-256:

`04e4db72629eaa786fdd182ac224f4fd6d68806f5f4fe5c1fb5f756dc4da11d7`

The VA-to-file-offset conversion was performed using the PE section table (not by treating the file as a flat memory image).

## Exact during-session sequence

Disassembly of the correctly mapped `.text` section:

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
004A617F  pop ebp
004A6180  pop edi
004A6181  pop esi
004A6182  pop ebx
004A6183  ret
```

Exact five bytes at the proposed insertion site `0x004A613D`:

`E8 0E A1 0A 00`

This is a relative CALL to `0x00550250` (CFont::DrawFonts). The next instruction begins at `0x004A6142`.

## Consequence for patching

The cleanest minimal patch is to replace only this five-byte CALL with a five-byte relative CALL to a local naked thunk. The thunk must:
1. Preserve all caller-visible general registers and EFLAGS around the MAP-page test/text-queue helper.
2. Return to the thunk after the helper.
3. Restore registers/flags exactly.
4. Execute the original `CFont::DrawFonts` call exactly once.
5. Return to `0x004A6142` with the original caller stack layout intact.

The thunk must not call the font flush twice and must not skip it. The helper must not run on a worker thread. The patch must be installed only after the module is fully loaded, after validating the executable base and the exact five expected bytes. Use VirtualProtect for the five-byte patch, restore protection, and flush the instruction cache. Installation should be idempotent and fail closed if the bytes do not match.

## Critical limitations before writing the ASI

- This note confirms the call-site bytes and control-flow boundary; it does not prove that a specific CFont state sequence renders the desired test string.
- The helper should set every property it relies on (scale, style, color, alignment, shadow) and should avoid assuming the vanilla state left by `0x5566E0`.
- Because CFont is shared global state, the first experiment should be a single obvious fixed string with a conservative style. The original draw-fonts call follows immediately, so the string should be submitted before the flush.
- A runtime MAP-page guard must read the confirmed manager field at `0x00869728` (manager base `0x00869630` + `0xF8`) and require value 6.
- Validate the patch and helper against the exact executable before exposing the ASI to the user.

## No change to existing probe

The read-only runtime probe remains unchanged. The MAP text test must be a separate build artifact and a separate workflow output.
