#include <iostream>
#include <Windows.h>

// Store the address where we install the hook (located in the VMT)
PUINT_PTR pHook;

// Signature of the hooked function
typedef void (*VirtualFunction)();

// Store the original content of the VMT
VirtualFunction originalFunction;

void VirtualFunctionHook()
{
	std::cout << "This function was hooked" << std::endl;

	// call the original function
	std::cout << "Calling the original function... " << std::endl;
	originalFunction();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{

	case DLL_PROCESS_ATTACH:
	{
		std::cout << std::endl
				  << "Installing the hook..." << std::endl;

		// Find base address of target program (.exe)
		UINT_PTR base = (UINT_PTR)GetModuleHandle(NULL);
		if (base == NULL)
		{
			std::cout << "Module handle not found" << std::endl;
			return FALSE;
		}

		// This is the offset of the call() function in the VMT of the Cat class
		// This offset can be found by analyzing the target with a decompiler like for example Ghidra
#ifndef _WIN64
		UINT_PTR offset = 0x337c;
#else
		UINT_PTR offset = 0x35a8;
#endif

		// A pointer to the address in the VMT that we will overwrite to install the hook
		pHook = (PUINT_PTR)(base + offset);
		std::cout << "Hook address: " << std::hex << pHook << std::endl;

		// Store the original contents of the VMT
		originalFunction = (VirtualFunction)*pHook;

		// Install hook
		DWORD oldProtect = 0;
		VirtualProtect(pHook, sizeof(UINT_PTR), PAGE_EXECUTE_READWRITE, &oldProtect);
		*pHook = (UINT_PTR)&VirtualFunctionHook;
		VirtualProtect(pHook, sizeof(UINT_PTR), oldProtect, &oldProtect);

		return TRUE;
	}
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
	{
		std::cout << std::endl
				  << "Uninstalling the hook..." << std::endl;

		DWORD oldProtect = 0;
		VirtualProtect(pHook, sizeof(UINT_PTR), PAGE_EXECUTE_READWRITE, &oldProtect);
		*pHook = (UINT_PTR)originalFunction;
		VirtualProtect(pHook, sizeof(UINT_PTR), oldProtect, &oldProtect);

		return TRUE;
	}
	}
	return TRUE;
}
