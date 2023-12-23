/*
    SOURCES:
        -

    PROJECT SETTINGS:
        - 

    INFO:
        - USAGE:
            This is a test application that has a class with virtual functions. 
            When the companion DLL is injected in this application,
            it will install a hook on the Virtual Method Table. This will overwrite the
            functions of this application with the functions from the DLL.

        -

*/

#include "Classes.h"
#include <Windows.h>

// When you refer to a derived class object using a pointer or a reference to the base class, 
// you can call a virtual function for that object and execute the derived class's version of the function.
void callFunctions(Base* pBase) {
    pBase->function1();
    pBase->function2();
}

int main()
{
    

	// Wait for an input to start using the functions (while waiting, you can inject the DLL with the VTable hook)
    system("pause");

    // Test to determine if hook is correctly installed
    Derived derived;
    Base* pBase2 = new Derived();

    // When refering to a derived class with a pointer to the base class, the VTable will be used if the function is defined as virtual in the base class
    pBase2->function1(); //not hooked
    pBase2->function2(); //hooked
    callFunctions(&derived); //not hooked, hooked

    //result: function2 is hooked both times

    // Wait to be able to read output
    system("pause");

	return 0;
}