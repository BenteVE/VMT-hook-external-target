/*
    SOURCES:
        - 

    PROJECT SETTINGS:
        - 

    INFO:
        - USAGE:
            This code creates a DLL. When the DLL is injected in the companion application, 
            it will install a hook on the Virtual Method Table. This will overwrite the
            functions of that application with our own functions.

        - 

*/
#include <iostream>
#include <Windows.h>


//we use this to store and call the original function inside our custom one
// when defining a thiscall, we always have to give the pointer ourselves
// (this pointer is automatically added by the compiler when the function is called through a class, when it is instead called with a pointer, you would have to add it manually)
//typedef void(__thiscall* VirtualFunction01_t)(void* thisptr);
//VirtualFunction01_t g_org_VirtualFunction01; //used to store original function address


// We will execute this function instead of the real one
// __thiscall calling convention is used on C++ class member functions on the x86 architecture (https://docs.microsoft.com/en-us/cpp/cpp/thiscall?view=msvc-170)
// __thiscall passes parameters pushed on stack, with an extra this pointer stored in ECX 
// __fastcall passes the first parameters stored in registers ECX en EDX, then pushed on stack
// we use __fastcall for our hook to so we can easily access the this pointer from the original __thiscall convention that the original function used
// if we would use __thiscall for our hook instead, we wouldn't get access to the pointer as easily because the this pointer is not explicitly mentionned in the parameters
void __fastcall hook(void* pEcx, void* pEdx) {
    //Base* object = (Base*)pEcx; //argument in register ecx is a pointer to this object
    std::cout << "This function was hooked" << std::endl;

    //call the original function
    //g_org_VirtualFunction01(thisptr);
}
/*
// werkt dit ook of kunnen we argument in Ecx niet bereiken met thiscall? (niet mogelijk om thiscall te definiëren buiten klasse?)
void __thiscall hook(void* Ecx, void* arg1, ...) {
    //Base* object = (Base*)pEcx; //argument in register ecx is a pointer to this object
    std::cout << "This function was hooked" << std::endl;

    //call the original function
    //g_org_VirtualFunction01(thisptr);
}
*/


DWORD WINAPI installVTableHook(PVOID base) {

    //find base address of module program 
    DWORD imageBase = (DWORD)GetModuleHandle(L"Virtual Method Table.exe");
    if (imageBase == NULL) {
        MessageBox(NULL, L"Base not found", L"Base not found", MB_OK);
        return FALSE;
    }
    
    DWORD offsetFunction2 = 0x3274; //found in Ghidra (2nd function in VTable of Derived class!)

    PDWORD pFunction2 = (PDWORD)(imageBase + offsetFunction2);
    std::cout << std::hex << imageBase << std::endl;
    std::cout << std::hex << offsetFunction2 << std::endl;
    std::cout << pFunction2 << std::endl;

    std::cout << std::hex << *pFunction2 << std::endl; //address of actual function in .exe

    // Get location of hook function, we will copy this location into the VTable
    void* pHookFnc = (void*)hook;

    std::cout << std::hex << pHookFnc << std::endl; //address of our function on top of this file

    // Install hook
    DWORD oldProtect = 0;
    VirtualProtect(pFunction2, sizeof(PDWORD), PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(pFunction2, &pHookFnc, sizeof(PDWORD));
    VirtualProtect(pFunction2, sizeof(PDWORD), oldProtect, &oldProtect);

    std::cout << std::hex << *pFunction2 << std::endl; //address in memory is changed to our address

    MessageBox(NULL, L"Hook installed", L"Hook installed", MB_OK);

    return TRUE;
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, NULL, installVTableHook, hModule, NULL, nullptr); break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

