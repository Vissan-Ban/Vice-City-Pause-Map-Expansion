#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Minimal visible-load test for GTA Vice City 1.0 (32-bit).
// No game addresses, hooks, patches, or game data are touched.

static DWORD WINAPI ShowLoadNotice(LPVOID)
{
    Sleep(2000);
    MessageBoxA(
        NULL,
        "VC ASI test loaded successfully.\n\nThis confirms the ASI loader loaded this 32-bit plugin.",
        "VC ASI LOAD TEST",
        MB_OK | MB_ICONINFORMATION
    );
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    (void)reserved;
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(instance);
        HANDLE thread = CreateThread(NULL, 0, ShowLoadNotice, NULL, 0, NULL);
        if (thread)
            CloseHandle(thread);
    }
    return TRUE;
}
