#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <string.h>

static const DWORD kMenuManagerAddress = 0x00869630;
static const int kMapPage = 6;
static volatile LONG g_running = 1;

static BOOL BuildExeLogPath(char* path, DWORD capacity)
{
    DWORD n = GetModuleFileNameA(NULL, path, capacity);
    if (n == 0 || n >= capacity) return FALSE;
    char* slash = strrchr(path, '\\');
    char* forward = strrchr(path, '/');
    if (!slash || (forward && forward > slash)) slash = forward;
    if (!slash) return FALSE;
    slash[1] = '\0';
    if (strlen(path) + sizeof("vc_pause_map_runtime_probe.log") > capacity) return FALSE;
    strcat_s(path, capacity, "vc_pause_map_runtime_probe.log");
    return TRUE;
}

static FILE* OpenProbeLog(char* path, DWORD capacity)
{
    if (BuildExeLogPath(path, capacity)) {
        FILE* f = NULL;
        if (fopen_s(&f, path, "a") == 0 && f) return f;
    }
    DWORD n = GetTempPathA(capacity, path);
    if (n && n < capacity && strlen(path) + sizeof("vc_pause_map_runtime_probe.log") <= capacity) {
        strcat_s(path, capacity, "vc_pause_map_runtime_probe.log");
        FILE* f = NULL;
        if (fopen_s(&f, path, "a") == 0 && f) return f;
    }
    return NULL;
}

static DWORD WINAPI ProbeThread(LPVOID)
{
    char path[MAX_PATH] = {};
    FILE* log = OpenProbeLog(path, MAX_PATH);
    if (!log) {
        OutputDebugStringA("VC Pause Map probe: unable to create log in EXE or TEMP directory.\n");
        return 0;
    }
    fprintf(log, "\r\n--- VC Pause Map read-only probe started ---\r\n");
    fprintf(log, "Log path: %s\r\n", path);
    fprintf(log, "Expected executable: VC 1.0 x86; manager=0x%08lX; MAP page=%d\r\n",
            (unsigned long)kMenuManagerAddress, kMapPage);
    HMODULE mainModule = GetModuleHandleA(NULL);
    fprintf(log, "Main module base: 0x%08lX\r\n", (unsigned long)(DWORD)mainModule);
    fflush(log);

    if ((DWORD)mainModule != 0x00400000) {
        fprintf(log, "ABORT: main module base is not 0x00400000; no game memory read performed.\r\n");
        fclose(log);
        return 0;
    }

    int lastPage = -999;
    DWORD lastSample = 0;
    while (InterlockedCompareExchange(&g_running, 1, 1)) {
        int page = -1, mouseX = 0, mouseY = 0, showMouse = 0;
        __try {
            const BYTE* manager = reinterpret_cast<const BYTE*>(kMenuManagerAddress);
            page = *reinterpret_cast<volatile const int*>(manager + 0xF8);
            mouseX = *reinterpret_cast<volatile const int*>(manager + 0x12C);
            mouseY = *reinterpret_cast<volatile const int*>(manager + 0x130);
            showMouse = *reinterpret_cast<volatile const BYTE*>(manager + 0x140);
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            fprintf(log, "READ ERROR: structured exception while reading menu manager; probe stopped.\r\n");
            fflush(log);
            break;
        }
        DWORD now = GetTickCount();
        if (page != lastPage) {
            fprintf(log, "PAGE %d -> %d at tick %lu\r\n", lastPage, page, (unsigned long)now);
            lastPage = page;
            fflush(log);
        }
        if (page == kMapPage && now - lastSample >= 1000) {
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
        else OutputDebugStringA("VC Pause Map probe: CreateThread failed.\n");
    }
    return TRUE;
}