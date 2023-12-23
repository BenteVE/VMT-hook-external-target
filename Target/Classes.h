#include <iostream>

// We will need these classes in the main file and in the DLL file.

class Base {
public:
	// Default constructor
	Base() { std::cout << "Base class created" << std::endl; };

	// Destructor 
	// Deleting a derived class object using a pointer of base class type that has a non-virtual destructor results in undefined behavior. 
	// To correct this situation, the base class should be defined with a virtual destructor.
	virtual ~Base() { std::cout << "Base class destroyed" << std::endl; };

	void function1() { std::cout << "Base function1 called" << std::endl; }
	// Virtual function: member function declared within a base class and redefined by a derived class.
	virtual void function2() { std::cout << "Base function2 called" << std::endl; }
};

class Derived : public Base {
public:
	//default constructor
	Derived() { std::cout << "Derived class created" << std::endl; };

	//Destructor
	~Derived() { std::cout << "Derived class destroyed" << std::endl; };

	// This function will not be executed because function1 in the Base class is not virtual
	void function1() { std::cout << "Derived function1 called" << std::endl; }
	// This function will be executed because function2 in the Base class is virtual
	void function2() { std::cout << "Derived function2 called" << std::endl; }
};
