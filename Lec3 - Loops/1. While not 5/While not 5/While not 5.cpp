// While not 5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

int main(){
	int clock = 0;
	int userInput = 0;
	while (userInput != 5 && clock < 10) {
		clock++;
		std::cout << "Please enter a number thats not 5.";
		std::cin >> userInput;
	}
	if (clock == 10)
		std::cout << "You entered 'not 5' TEN times!";
	else {
		std::cout << "You entered 5! TWAT!";
	}
	
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
