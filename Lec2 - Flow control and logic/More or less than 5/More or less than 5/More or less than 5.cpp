// More or less than 5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

int main()
{
	int userNum;
    std::cout << "Please enter a number between 0 and 10!\n";
	std::cin >> userNum;
	if (userNum == 5 ){
		std::cout << "You have entered 5!\n";
	}
	else if (userNum < 5 ) {
		std::cout << "You have entered less than 5!\n";
	}
	else if (userNum > 5 ) {
		std::cout << "You have entered more than 5!\n";
	}
	else {
		std::cout << "You have entered an invalid number!\n";
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
