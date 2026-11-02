#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

static const DWORD kMenuManagerAddress = 0x00869630;
static const int kMapPage = 6;
static volatile LONG g_running = 1;

static DWORD WINAPI ProbeThread(LPVOID)
{
    char path[MAX_PATH] = {};
    GetCurrentDirectoryA(MAX_PATH, path);
    size_t used = strlen(path);
    if (used + 40 < MAX_PATH) {
        if (used && path[used - 1] != '\\' && path[used - 1] != '/')
            strcat_s(path, MAX_PATH, "\\");
        strcat_s(path, MAX_PATH, "vc_pause_map_runtime_probe.log");
    } else {
        return 0;
    }

    FILE* log = NULL;
    if (fopen_s(&log, path, "a") != 0 || !log)
        return 0;

    fprintf(log, "\r\n--- VC Pause Map read-only probe started ---\r\n");
    fprintf(log, "Expected executable: VC 1.0 x86; manager=0x%08X; MAP page=%d\r\n",
            kMenuManagerAddress, kMapPage);
    fflush(log);

    if (reinterpret_cast<DWORD>(GetModuleHandleA(NULL)) != 0x00400000) {
        fprintf(log, "ABORT: main module base is not 0x00400000; no game memory read performed.\r\n");
        fclose(log);
        return 0;
    }

    int lastPage = -999;
    DWORD lastSample = 0;
    while (InterlockedCompareExchange(&g_running, 1, 1)) {
        int page = -1, mouseX = 0, mouseY = 0, showMouse = 0;
        BOOL readOk = FALSE;

        __try {
            const BYTE* manager = reinterpret_cast<const BYTE*>(kMenuManagerAddress);
            page = *reinterpret_cast<volatile const int*>(manager + 0xF8);
            mouseX = *reinterpret_cast<volatile const int*>(manager + 0x12C);
            mouseY = *reinterpret_cast<volatile const int*>(manager + 0x130);
            showMouse = *reinterpret_cast<volatile const BYTE*>(manager + 0x140);
            readOk = TRUE;
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            fprintf(log, "READ ERROR: structured exception while reading menu manager; probe stopped.\r\n");
            fflush(log);
            break;
        }

        DWORD now = GetTickCount();
        if (!readOk) break;

        if (page != lastPage) {
            fprintf(log, "PAGE %d -> %d at tick %lu\r\n", lastPage, page, (unsigned long)now);
            lastPage = page;
            fflush(log);
        }

        if (page == kMapPage && (now - lastSample >= 1000)) {
            fprintf(log, "MAP mouse=(%d,%d) showMouse=%d tick=%lu\r\n",
                    mouseX, mouseY, showMouse, (unsigned long)now);
            fflush(log);
            lastSample = now;
        }

        Sleep(100);
    }

    fprintf(log, "--- probe stopped ---\r\n");
    fclose(log);
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    (void)reserved;
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(instance);
        HANDLE thread = CreateThread(NULL, 0, ProbeThread, NULL, 0, NULL);
        if (thread) CloseHandle(thread);
    }
    return TRUE;
}
