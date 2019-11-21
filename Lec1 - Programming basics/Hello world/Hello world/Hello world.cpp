// Hello world.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

int main()
{
	using namespace std;

    cout << "Hello World!\n";

	int n, k, c, space = 1; // all equal 1
	cout << "Enter the number of Rows..." << endl; // how big the diamond to be
	cin >> n;
	space = n - 1; //userinput - 1 is the space amount
	for (k = 1; k <= n; k++) // while less than or equal to user input
	{
		for (c = 1; c <= space; c++) // while less than or equal to userinput - 1 (or space)
		{
			cout << " ";
		}
		space--;					// decrease space amount
		for (c = 1; c <= 2 * k - 1; c++)
		{
			cout << "*";
		}
		cout << endl;
	}
	space = 1;
	for (k = 1; k <= n; k++)
	{
		for (c = 1; c <= space; c++)
		{
			cout << " ";
		}
		space++;
		for (c = 1; c <= 2 * (n - k) - 1; c++)
		{
			cout << "*";
		}
		cout << endl;
	}
	return 0;
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
