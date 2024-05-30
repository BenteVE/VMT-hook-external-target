// A simple application that uses a Virtual Method Table

#include "Animal.h"
#include <Windows.h>
#include <vector>

#include <tchar.h> // using _tmain macro
#include <conio.h> // using _getch

int main()
{
	// Creating a vector of animals
	std::vector<Animal *> animals;

	// Using getch in a while loop to provide options
	int ch = ' ';
	while (true)
	{
		switch (ch)
		{
		case 'Q':
			return 0;
		case 'A':
		{
			std::cout << std::endl;
			Animal a;
			animals.push_back(&a);
			break;
		}
		case 'C':
		{
			std::cout << std::endl;
			Cat c;
			animals.push_back(&c);
			break;
		}
		case 'D':
		{
			std::cout << std::endl;
			Dog d;
			animals.push_back(&d);
			break;
		}
		case 'M':
		{
			std::cout << std::endl
					  << "Letting all animals make their move" << std::endl;
			for (Animal *animal : animals)
			{
				// Note: the move() method in the Animal class is not defined as virtual,
				// so all animals (cats and dogs) will move like generic animals
				animal->move();
			}
			break;
		}
		case 'N':
		{
			std::cout << std::endl
					  << "Letting all animals make their noise" << std::endl;
			for (Animal *animal : animals)
			{
				// Note: the call() method in the Animal class is defined as virtual,
				// so the VMT will be used to determine which function should be executed at runtime
				animal->call();
			}
			break;
		}
		default:
		{
			std::cout << std::endl;
			std::cout << "Press 'A' to create a generic animal" << std::endl;
			std::cout << "Press 'C' to create a cat" << std::endl;
			std::cout << "Press 'D' to create a dog" << std::endl;
			std::cout << "Press 'M' to let the animals move" << std::endl;
			std::cout << "Press 'N' to let the animals make their noise" << std::endl;
			std::cout << "Press 'Q' to quit" << std::endl;
			break;
		}
		}

		ch = _gettch();
		ch = toupper(ch);
	}
}