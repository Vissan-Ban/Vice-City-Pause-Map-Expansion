#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Minimal 32-bit Vice City ASI build probe.
// This DLL intentionally does not hook or modify the game.
BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    (void)instance;
    (void)reason;
    (void)reserved;
    return TRUE;
}
