#include <Windows.h>
#include "hooks/Hooks.hpp"

DWORD WINAPI MainThread(LPVOID lpParam)
{
    Hooks::Initialize();
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        CreateThread(0, 0, MainThread, hModule, 0, 0);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        Hooks::Shutdown();
    }
    return TRUE;
}
