#include <iostream>

int main()
{
	int userNum;
	int totalNum = 0;
    std::cout << "Please enter a number from 1-9.";
	std::cin >> userNum;

	for (int i = 0; i < userNum + 1; i++) {
		std::cout << i << " ";
		totalNum = totalNum + i;
		std::cout << "Current total: " <<totalNum;
	}
}

