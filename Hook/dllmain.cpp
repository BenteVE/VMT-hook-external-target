#include <iostream>
#include <Windows.h>

//we use this to store and call the original function inside our custom one
// when defining a thiscall, we always have to give the pointer ourselves
// (this pointer is automatically added by the compiler when the function is called through a class, when it is instead called with a pointer, you would have to add it manually)
typedef void(__thiscall* VirtualFunction)(void* thisptr);
VirtualFunction g_org_VirtualFunction; //used to store original function address

// We will execute this function instead of the real one
// __thiscall calling convention is used on C++ class member functions on the x86 architecture (https://docs.microsoft.com/en-us/cpp/cpp/thiscall?view=msvc-170)
// __thiscall passes parameters pushed on stack, with an extra this pointer stored in ECX 
// __fastcall passes the first parameters stored in registers ECX en EDX, then pushed on stack
// we use __fastcall for our hook to so we can easily access the this pointer from the original __thiscall convention that the original function used
// if we would use __thiscall for our hook instead, we wouldn't get access to the pointer as easily because the this pointer is not explicitly mentionned in the parameters
void __fastcall hook(void* pEcx, void* pEdx) {
	//argument in register ecx is a pointer to this object
	std::cout << "This function was hooked" << std::endl;
	std::cout << "Address of the object: " << std::hex << pEcx << std::endl;
	
	//call the original function
	std::cout << "Calling the original function... " << std::endl;
	g_org_VirtualFunction(pEcx);
}

DWORD WINAPI installVTableHook() {

	// Find base address of target program (.exe)
	UINT_PTR base = (UINT_PTR)GetModuleHandle(NULL);
	if (base == NULL) {
		std::cout << "Module handle not found" << std::endl;
		return FALSE;
	}
	std::cout << "Module base: " << std::hex << base << std::endl;

	// For the target program with x86 architecture,
	// this is the offset of the call() function in the VMT of the Cat class
	// This offset can be found by analyzing the program with for example Ghidra
	UINT_PTR offset = 0x337c;
	std::cout << "Offset: " << std::hex << offset << std::endl;

	// A pointer to the address in the VMT that we will overwrite to install the hook
	PUINT_PTR pHook = (PUINT_PTR)(base + offset);
	std::cout << "Hook address: " << std::hex << pHook << std::endl;

	// Check the original content of the VMT, this is the true address of function2() of the derived class
	std::cout << "Content of the VMT: " << std::hex << *pHook << std::endl;

	// Store the original contents of the VMT:
	g_org_VirtualFunction = (VirtualFunction)*pHook;

	// Install hook
	DWORD oldProtect = 0;
	VirtualProtect(pHook, sizeof(UINT_PTR), PAGE_EXECUTE_READWRITE, &oldProtect);
	*pHook = (UINT_PTR)&hook;
	VirtualProtect(pHook, sizeof(UINT_PTR), oldProtect, &oldProtect);

	// Check if the content is correctly overwritten
	std::cout << "Content of the VMT: " << std::hex << *pHook << std::endl;

	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{

	case DLL_PROCESS_ATTACH:
	{
		std::cout << std::endl << "Installing the hook..." << std::endl;

		// install hook
		CreateThread(nullptr, NULL, LPTHREAD_START_ROUTINE(installVTableHook), NULL, NULL, nullptr);

		return TRUE;
	}
	case DLL_THREAD_ATTACH: break;
	case DLL_THREAD_DETACH: break;
	case DLL_PROCESS_DETACH: {
		std::cout << std::endl << "Uninstalling the hook..." << std::endl;
		return TRUE;
	}
	}
	return TRUE;
}

