#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <psapi.h>

#include "SDK.h"

// Globals
HANDLE g_process_handle = NULL;
uintptr_t g_module_base = 0;

// Offsets from user
const uintptr_t OFFSET_LOCALPLAYER = 0x6CC8390;

// Memory reading functions
template<typename T>
T read_memory(uintptr_t address) {
    T buffer;
    if (g_process_handle && address != 0 && ReadProcessMemory(g_process_handle, (LPCVOID)address, &buffer, sizeof(T), NULL)) {
        return buffer;
    }
    return T{};
}

// Process functions
DWORD get_process_id(const TCHAR* process_name) {
    DWORD process_id = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 process_entry;
        process_entry.dwSize = sizeof(process_entry);
        if (Process32First(snapshot, &process_entry)) {
            do {
                if (_tcsicmp(process_entry.szExeFile, process_name) == 0) {
                    process_id = process_entry.th32ProcessID;
                    break;
                }
            } while (Process32Next(snapshot, &process_entry));
        }
        CloseHandle(snapshot);
    }
    return process_id;
}

uintptr_t get_module_base_address(DWORD process_id, const TCHAR* module_name) {
    uintptr_t module_base_address = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process_id);
    if (snapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 module_entry;
        module_entry.dwSize = sizeof(module_entry);
        if (Module32First(snapshot, &module_entry)) {
            do {
                if (_tcsicmp(module_entry.szModule, module_name) == 0) {
                    module_base_address = (uintptr_t)module_entry.modBaseAddr;
                    break;
                }
            } while (Module32Next(snapshot, &module_entry));
        }
        CloseHandle(snapshot);
    }
    return module_base_address;
}

int main() {
    const TCHAR* process_name = _T("GroundBranch-Win64-Shipping.exe");
    DWORD process_id = 0;

    _tprintf(_T("Searching for process: %s...\n"), process_name);
    while (process_id == 0) {
        process_id = get_process_id(process_name);
        Sleep(1000);
    }
    _tprintf(_T("Process found! PID: %d\n"), process_id);

    g_process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
    if (g_process_handle == NULL) {
        _ftprintf(stderr, _T("Failed to open process. Error: %d\n"), GetLastError());
        return 1;
    }

    g_module_base = get_module_base_address(process_id, process_name);
    if (g_module_base == 0) {
        _ftprintf(stderr, _T("[FATAL] Failed to get module base address. The program cannot continue.\n"));
        system("pause");
        CloseHandle(g_process_handle);
        return 1;
    }

    // --- Main Cheat Loop ---
    const uintptr_t OFFSET_PLAYERCONTROLLER = 0x30;
    const uintptr_t OFFSET_PAWN = 0x5D0;
    const uintptr_t OFFSET_HEALTH = 0x594;
    const uintptr_t OFFSET_STAMINA = 0x744;

    std::cout << "Starting main cheat loop. Reading local player data..." << std::endl;
    while (true) {
        uintptr_t local_player_ptr_addr = g_module_base + OFFSET_LOCALPLAYER;
        uintptr_t local_player_obj = read_memory<uintptr_t>(local_player_ptr_addr);
        if (local_player_obj == 0) {
            std::cout << "Could not find LocalPlayer object. Retrying..." << std::endl;
            Sleep(1000);
            continue;
        }

        uintptr_t player_controller_addr = read_memory<uintptr_t>(local_player_obj + OFFSET_PLAYERCONTROLLER);
        if (player_controller_addr == 0) {
            std::cout << "Could not find PlayerController object. Retrying..." << std::endl;
            Sleep(1000);
            continue;
        }

        uintptr_t character_addr = read_memory<uintptr_t>(player_controller_addr + OFFSET_PAWN);
        if (character_addr == 0) {
            std::cout << "Could not find Character object. Retrying..." << std::endl;
            Sleep(1000);
            continue;
        }

        int32_t health = read_memory<int32_t>(character_addr + OFFSET_HEALTH);
        float stamina = read_memory<float>(character_addr + OFFSET_STAMINA);

        system("cls"); // Clear console
        std::cout << "--- Local Player Stats ---" << std::endl;
        std::cout << "Health: " << health << std::endl;
        std::cout << "Stamina: " << stamina << std::endl;
        std::cout << "--------------------------" << std::endl;

        Sleep(500);
    }

    CloseHandle(g_process_handle);
    return 0;
}