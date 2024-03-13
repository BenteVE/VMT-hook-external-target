#include <iostream>

class Animal {
public:
	// Default constructor
	Animal() { std::cout << "Animal created" << std::endl; };

	void move() { std::cout << "Generic animal move" << std::endl; }
	virtual void call() { std::cout << "Generic animal call" << std::endl; }
};

class Cat : public Animal {
public:
	Cat() { std::cout << "Cat created" << std::endl; };

	void move() { std::cout << "Move like a cat" << std::endl; }
	void call() { std::cout << "Meow like a cat" << std::endl; }
};

class Dog : public Animal {
public:
	Dog() { std::cout << "Dog created" << std::endl; };

	void move() { std::cout << "Move like a dog" << std::endl; }
	void call() { std::cout << "Bark like a dog" << std::endl; }
};

