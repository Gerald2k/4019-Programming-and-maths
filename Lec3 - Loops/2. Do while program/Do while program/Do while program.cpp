#include <iostream>

int main()
{
	int userNum = 0;
	do {
		std::cout << "Please input a valid input between 1 and 10.";
		std::cin >> userNum;

	} while (userNum < 0 || userNum > 11);
		
	
}

