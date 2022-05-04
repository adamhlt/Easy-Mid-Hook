#include <Windows.h>

#include "Global.h"
#include "Detours/include/detours.h"

/*
 * detours.h do not contain these structs so we need to redifine it here.
 * You can find the original definition in detours.cpp in the library source code.
 */
struct DETOUR_ALIGN
{
    BYTE    obTarget : 3;
    BYTE    obTrampoline : 5;
};

struct DETOUR_INFO
{
    // An X64 instuction can be 15 bytes long.
    // In practice 11 seems to be the limit.
    BYTE            rbCode[30];     // target code + jmp to pbRemain.
    BYTE            cbCode;         // size of moved target code.
    BYTE            cbCodeBreak;    // padding to make debugging easier.
    BYTE            rbRestore[30];  // original target code.
    BYTE            cbRestore;      // size of original target code.
    BYTE            cbRestoreBreak; // padding to make debugging easier.
    DETOUR_ALIGN    rAlign[8];      // instruction alignment array.
    PBYTE           pbRemain;       // first instruction after moved code. [free list]
    PBYTE           pbDetour;       // first instruction of detour function.
    BYTE            rbCodeIn[8];    // jmp [pbDetour]
};

using lpGetValueFnc = INT64 (*)();

void InitHook()
{
    dwNumber = 1337; //Set the new number to display.

    PDETOUR_TRAMPOLINE lpTrampolineData = {}; //Datas to sotre information about hooking
    const auto lpGetValue = (LPVOID)((DWORD_PTR)GetModuleHandleA("Test.exe") + 0x117CB); //Address of the instruction I want to replace ".text:00000001400117CB : mov eax, 0Ah", this is the DEBUG build of Test.exe
                                                                                         //I am lazy but you should use pattern scanning to retrieve the address

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttachEx((PVOID*)&lpGetValue, (PVOID)&hkGetValue, &lpTrampolineData, nullptr, nullptr); //Use DetourAtttachEx to retrieve information about the hook
    DetourTransactionCommit();

    const auto lpDetourInfo = (DETOUR_INFO*)lpTrampolineData;
    lpRemain = lpDetourInfo->pbRemain; //Retrieve the address to jump back the original function
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        InitHook();
    }

    return TRUE;
}