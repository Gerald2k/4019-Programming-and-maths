#include <iostream>

using namespace std;

int main()
{
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