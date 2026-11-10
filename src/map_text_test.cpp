#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include <string.h>

// Experimental GTA Vice City PC 1.0 x86 MAP-page fixed-text test.
typedef void (__cdecl *FontVoidFn)();
typedef void (__cdecl *FontShortFn)(short);
typedef void (__cdecl *FontFloatFn)(float);
typedef void (__cdecl *FontScaleFn)(float, float);
struct Rgba { unsigned char r, g, b, a; };
typedef void (__cdecl *FontColorFn)(const Rgba*);
typedef void (__cdecl *FontPrintFn)(float, float, const wchar_t*);

static const DWORD kExpectedBase = 0x00400000;
static const DWORD kPatchSite = 0x004A613D;
static const DWORD kOriginalDrawFonts = 0x00550250;
static const DWORD kPageAddress = 0x00869728;
static const int kMapPage = 6;
static const BYTE kExpected[5] = { 0xE8, 0x0E, 0xA1, 0x0A, 0x00 };

static void QueueFixedText()
{
    __try {
        if (*reinterpret_cast<volatile int*>(kPageAddress) != kMapPage) return;
        const Rgba textColor = { 255, 80, 210, 255 };
        const Rgba shadowColor = { 0, 0, 0, 255 };
        reinterpret_cast<FontVoidFn>(0x00550020)(); // SetPropOn
        reinterpret_cast<FontVoidFn>(0x005500D0)(); // SetBackgroundOff
        reinterpret_cast<FontVoidFn>(0x00550110)(); // SetCentreOff
        reinterpret_cast<FontVoidFn>(0x00550040)(); // SetRightJustifyOff
        reinterpret_cast<FontFloatFn>(0x00550100)(640.0f); // SetWrapx
        reinterpret_cast<FontColorFn>(0x00550170)(&textColor);
        reinterpret_cast<FontColorFn>(0x0054FF30)(&shadowColor);
        reinterpret_cast<FontShortFn>(0x0054FF20)(1);
        reinterpret_cast<FontShortFn>(0x0054FFE0)(2);
        reinterpret_cast<FontScaleFn>(0x00550230)(0.64f, 1.28f);
        reinterpret_cast<FontPrintFn>(0x00551040)(32.0f, 48.0f, L"MAP TEXT TEST");
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        OutputDebugStringA("VC MAP text test: exception while queuing CFont text.\n");
    }
}

__declspec(naked) static void DrawFontsThunk()
{
    __asm {
        pushfd
        pushad
        call QueueFixedText
        popad
        popfd
        call dword ptr [kOriginalDrawFonts]
        ret
    }
}

static bool InstallHook()
{
    if ((DWORD)GetModuleHandleA(NULL) != kExpectedBase) return false;
    BYTE* site = reinterpret_cast<BYTE*>(kPatchSite);
    __try {
        if (memcmp(site, kExpected, sizeof(kExpected)) != 0) return false;
    } __except (EXCEPTION_EXECUTE_HANDLER) { return false; }

    DWORD oldProtect = 0;
    if (!VirtualProtect(site, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) return false;
    intptr_t displacement = reinterpret_cast<intptr_t>(&DrawFontsThunk) - static_cast<intptr_t>(kPatchSite + 5);
    if (displacement < INT32_MIN || displacement > INT32_MAX) {
        DWORD ignored = 0; VirtualProtect(site, 5, oldProtect, &ignored); return false;
    }
    BYTE patch[5] = { 0xE8, 0, 0, 0, 0 };
    *reinterpret_cast<int32_t*>(patch + 1) = static_cast<int32_t>(displacement);
    memcpy(site, patch, sizeof(patch));
    FlushInstructionCache(GetCurrentProcess(), site, sizeof(patch));
    DWORD ignored = 0; VirtualProtect(site, 5, oldProtect, &ignored);
    return true;
}

static DWORD WINAPI InstallThread(LPVOID)
{
    for (int i = 0; i < 120; ++i) {
        if ((DWORD)GetModuleHandleA(NULL) == kExpectedBase) {
            OutputDebugStringA(InstallHook() ? "VC MAP fixed-text test: hook installed.\n"
                                             : "VC MAP fixed-text test: hook installation failed.\n");
            return 0;
        }
        Sleep(250);
    }
    OutputDebugStringA("VC MAP fixed-text test: timed out waiting for VC 1.0 x86.\n");
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(instance);
        HANDLE thread = CreateThread(NULL, 0, InstallThread, NULL, 0, NULL);
        if (thread) CloseHandle(thread);
    }
    return TRUE;
}