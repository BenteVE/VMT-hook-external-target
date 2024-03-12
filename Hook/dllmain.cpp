#include <iostream>
#include <Windows.h>

#include "Console.h"

Console console;

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


DWORD WINAPI installVTableHook() {

	//find base address of module program 
	UINT_PTR base = (UINT_PTR)GetModuleHandle(NULL);
	if (base == NULL) {
		fprintf(console.stream, "Module handle not found\n");
		return FALSE;
	}

	// For the target program with x86 architecture,
	// this is the offset of the second function in the Virtual Method Table of the Derived class
	// This offset can be found by analyzing the program with for example Ghidra
	UINT_PTR offset = 0x3274;

	// A pointer to the address in the VMT that we will overwrite to install the hook
	PUINT_PTR pHook = (PUINT_PTR)(base + offset);

	// Check the original content of the VMT, this is the true address of function2() of the derived class
	fprintf(console.stream, "Content of the VMT: %x", *pHook);

	// Install hook
	DWORD oldProtect = 0;
	VirtualProtect(pHook, sizeof(UINT_PTR), PAGE_EXECUTE_READWRITE, &oldProtect);
	*pHook = (UINT_PTR)&hook;
	VirtualProtect(pHook, sizeof(UINT_PTR), oldProtect, &oldProtect);

	// Check if the content is correctly overwritten
	fprintf(console.stream, "Content of the VMT: %x", *pHook);

	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{

	case DLL_PROCESS_ATTACH:
	{
		// The DisableThreadLibraryCalls function lets a DLL disable the DLL_THREAD_ATTACH and DLL_THREAD_DETACH notification calls.
		// This can be a useful optimization for multithreaded applications that have many DLLs, frequently createand delete threads, 
		// and whose DLLs do not need these thread - level notifications of attachment/detachment.
		DisableThreadLibraryCalls(hModule);

		if (!console.open()) {
			// Indicate DLL loading failed
			return FALSE;
		}

		// install hook
		CreateThread(nullptr, NULL, LPTHREAD_START_ROUTINE(installVTableHook), NULL, NULL, nullptr);

		return TRUE;
	}
	case DLL_THREAD_ATTACH: break;
	case DLL_THREAD_DETACH: break;
	case DLL_PROCESS_DETACH: {
		fprintf(console.stream, "Uninstalling the hook ...\n");

		// Open a MessageBox to allow reading the output
		MessageBoxW(NULL, L"Press Ok to close", L"Closing", NULL);

		return TRUE;
	}
	}
	return TRUE;
}

