#include "dllmain.h"
#include "Log.h"
#include "Hook.h"
#include <vector>
#include <cstdlib>
#include "client/common/client/game/ClientInstance.h"
#include "Mod.h"
#include "AmethystRuntime.h"

AmethystRuntime g_amethyst;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Main, NULL, 0, NULL);
    }

    return TRUE;
}

DWORD WINAPI Main() {
    Log::InitializeConsole();
    Log::Info("Minecraft Base Address: 0x{:x}\n", GetMinecraftBaseAddress());
    Log::Info("Minecraft Size: 0x{:x}\n", GetMinecraftSize());

    try {
        g_amethyst.LoadMods();
        g_amethyst.RunMods();
    }
    catch (std::exception e) {
        Log::Error("[AmethystRuntime] {}\n", e.what());
        ShutdownWait();
        return 1;
    }

    g_amethyst.Shutdown();
    Shutdown();
    return 0;
}

DWORD __stdcall EjectThread(LPVOID lpParameter) {
    ExitProcess(0);
}

void Shutdown() {
    g_amethyst.Shutdown();
    Log::DestroyConsole();
    MH_Uninitialize();

    CreateThread(0, 0, EjectThread, 0, 0, 0);
}

void ShutdownWait() {
    Log::Info("Press Numpad0 to close...\n");

    while (1) {
        Sleep(10);
        if (GetAsyncKeyState(VK_NUMPAD0)) break;
    }

    Shutdown();
}