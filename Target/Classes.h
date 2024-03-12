#include <iostream>

class Base {
public:
	// Default constructor
	Base() { std::cout << "Base class created" << std::endl; };

	// Destructor 
	// Deleting a derived class object using a pointer of base class type 
	// that has a non-virtual destructor results in undefined behavior. 
	// To correct this situation, the base class should be defined with a virtual destructor.
	virtual ~Base() { std::cout << "Base class destroyed" << std::endl; };

	void function1() { std::cout << "Base function1 called" << std::endl; }
	virtual void function2() { std::cout << "Base function2 called" << std::endl; }
	// Note: only function2() is defined as virtual
};

class Derived : public Base {
public:
	// Default constructor
	Derived() { std::cout << "Derived class created" << std::endl; };

	// Destructor
	~Derived() { std::cout << "Derived class destroyed" << std::endl; };

	void function1() { std::cout << "Derived function1 called" << std::endl; }
	void function2() { std::cout << "Derived function2 called" << std::endl; }
};
