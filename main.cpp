#include <iostream>
#include <Windows.h>

#include "detours.h"
#include "Globals.h"

#if _WIN32 || _WIN64
#if _WIN64
#pragma comment(lib, "Detours/lib.X64/detours.lib")
#else
#pragma comment(lib, "Detours/lib.X86/detours.lib")
#endif
#endif

char* GetClipboardText(size_t& len) {
    char* out_str = nullptr;
    if (!OpenClipboard(nullptr)) {
        return nullptr;
    }

    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData == nullptr) {
        CloseClipboard();
        return nullptr;
    }
    char* pszText = static_cast<char*>(GlobalLock(hData));
    if (pszText == nullptr) {
        CloseClipboard();
        return nullptr;
    }

    len = strlen(pszText) + 2;
    out_str = (char*)malloc(len * sizeof(char));
    if (out_str == nullptr) {
        GlobalUnlock(hData);
        CloseClipboard();
        return nullptr;
    }
    memset(out_str, 0, len * sizeof(char));
    memcpy(out_str, pszText, len);

    GlobalUnlock(hData);
    CloseClipboard();

    return out_str;
}

extern "C" void PerformCopyPaste() {
    /* 0xA2 = Ctrl Probably; and abusing the last byte to excute the below block once;
     * the game can call this hook multiple times */
    if (KeyState >> 32 == 0xA2 && (KeyState & 0xff) == 0x1) {
        size_t len = 0;
        char* clipboard_text = GetClipboardText(len);
        if (clipboard_text) {
            *reinterpret_cast<uintptr_t*>(CMInstance + 0x40) =
                reinterpret_cast<uintptr_t>(clipboard_text);
            *reinterpret_cast<uintptr_t*>(CMInstance + 0x40 + 0x8) =
                reinterpret_cast<uintptr_t>(clipboard_text) + 0x1;

            char* copy_clipboard_text = (char*)malloc(len * sizeof(char));
            if (!copy_clipboard_text) {
                return;
            }
            memcpy(copy_clipboard_text, clipboard_text, len);
            *reinterpret_cast<uintptr_t*>(CMInstance + 0x60) =
                reinterpret_cast<uintptr_t>(copy_clipboard_text);
            *reinterpret_cast<uintptr_t*>(CMInstance + 0x60 + 0x8) =
                reinterpret_cast<uintptr_t>(copy_clipboard_text) + 0x2;
        }
    }
}

extern "C" void hooked_CopyPaste();

DWORD WINAPI DllThread(HMODULE hModule) {
    DisableThreadLibraryCalls((HMODULE)hModule);
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    MainHandle = reinterpret_cast<uintptr_t>(GetModuleHandle(L"bf1.exe"));

    if (MainHandle) {
        CopyPasteHook += MainHandle;
        JumpBackFromCopyPasteHook = CopyPasteHook + 0x5;

        DetourAttach(&(PVOID&)CopyPasteHook, hooked_CopyPaste);
    }

    DetourTransactionCommit();

    while (true) {
        if (GetKeyState(VK_F5) & 0x8000) {
            break;
        }
        Sleep(20);
    }

    DetourTransactionBegin();
    DetourDetach(&(PVOID&)CopyPasteHook, hooked_CopyPaste);
    DetourTransactionCommit();

    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {

    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        HANDLE handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DllThread, hModule, 0, NULL);
        if (handle) {
            CloseHandle(handle);
        }
    }
    return TRUE;
}