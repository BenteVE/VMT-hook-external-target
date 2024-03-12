// A simple application that uses a Virtual Method Table

#include "Classes.h"
#include <Windows.h>

void callFunctions(Base* pBase) {
	pBase->function1();
	pBase->function2();
}

int main()
{
	// Wait for an input to start using the functions 
	// while waiting, we can inject the DLL that installs the Virtual Method Table (VMT) hook
	system("pause");

	// Here we are calling the functions in the derived class with a pointer to the base class
	// In this case, the VMT will be used for functions defined as virtual in the base class
	// Note: in the Base class, only function2() is defined as virtual, 
	// so only function2() will appear in the VMT, and we can only hook that function
	std::cout << "Calling the functions with a pointer to the base class:" << std::endl;
	Base* pBase = new Derived();
	pBase->function1();
	pBase->function2();

	Derived derived;
	callFunctions(&derived);

	// When calling function2() without a pointer to the base class, 
	// the VMT will not be used, so the hook will not be executed
	std::cout << "Calling the functions without a pointer to the base class:" << std::endl;
	derived.function1();
	derived.function2();

	// Wait to be able to read output before closing
	system("pause");

	return 0;
}