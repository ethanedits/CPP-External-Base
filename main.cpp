#include <Windows.h>
#include<TlHelp32.h>
#include <iostream>
#include <tchar.h>
#include <vector>
#include <stdlib.h>

using namespace std;

DWORD GetModuleBaseAddress(TCHAR* lpszModuleName, DWORD pID) {
    DWORD dwModuleBaseAddress = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID); // make snapshot of all modules within process
    MODULEENTRY32 ModuleEntry32 = { 0 };
    ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

    if (Module32First(hSnapshot, &ModuleEntry32)) //store first Module in ModuleEntry32
    {
        do {
            if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0) // if Found Module matches Module we look for -> done!
            {
                dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
                break;
            }
        } while (Module32Next(hSnapshot, &ModuleEntry32)); // go through Module entries in Snapshot and store in ModuleEntry32


    }
    CloseHandle(hSnapshot);
    return dwModuleBaseAddress;
}

DWORD GetPointerAddress(HWND hwnd, DWORD gameBaseAddr, DWORD address, vector<DWORD> offsets)
{
    DWORD pID = NULL; // Game process ID
    GetWindowThreadProcessId(hwnd, &pID);
    HANDLE phandle = NULL;
    phandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    if (phandle == INVALID_HANDLE_VALUE || phandle == NULL);

    DWORD offset_null = NULL;
    ReadProcessMemory(phandle, (LPVOID*)(gameBaseAddr + address), &offset_null, sizeof(offset_null), 0);
    DWORD pointeraddress = offset_null; // the address we need
    for (int i = 0; i < offsets.size() - 1; i++) // we dont want to change the last offset value so we do -1
    {
        ReadProcessMemory(phandle, (LPVOID*)(pointeraddress + offsets.at(i)), &pointeraddress, sizeof(pointeraddress), 0);
    }
    return pointeraddress += offsets.at(offsets.size() - 1); // adding the last offset
}

int main()
{
    HWND hwnd_AC = FindWindowA(NULL, "AssaultCube"); //Getting the process (Name of the window)

    if (hwnd_AC != FALSE);
    DWORD pID = NULL;
    GetWindowThreadProcessId(hwnd_AC, &pID);
    HANDLE phandle = NULL;
    phandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    if (phandle == INVALID_HANDLE_VALUE || phandle == NULL);

    char gameName1[] = "ac_client.exe"; // Getting the module (Found in Cheat Engine)
    DWORD gameBaseAddress1 = GetModuleBaseAddress(_T(gameName1), pID);

    //////////////////////////
    //       OFFSETS        //
    //////////////////////////

    //Ammo
    DWORD ammoAddr = 0x0010A280; //Ammo Address (This is a pointer in CE)
    vector<DWORD> ammoOffsets{ 0xE1C, 0x6C, 0x4E0 }; //Ammo's  Offsets (The pointers offsets in CE)
    DWORD ammoPtrAddr = GetPointerAddress(hwnd_AC, gameBaseAddress1, ammoAddr, ammoOffsets);

    //////////////////////////
    //    Writing Memory    //
    //////////////////////////

    while (true)
    {
        int ammo = NULL;
        ammo = 999;
        WriteProcessMemory(phandle, (LPVOID*)(ammoPtrAddr), &ammo, 4, 0);
    }
}
