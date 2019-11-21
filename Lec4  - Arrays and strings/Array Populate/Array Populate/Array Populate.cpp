#include <iostream>

/*int main()
{
	int hundredArray[100];
	int arrayValue = 0;
	for (int i = 0; i < 100; i++) {
		hundredArray[arrayValue] = i;
		std::cout << hundredArray[arrayValue];                      // FIRST PROGRAM FOR 1 ARRAY
	}
}*/

/*int main()
{
	int hundredArray[100];
	int twoHundredArray[100];
	int arrayValue = 0;
	int arrayValue2 = 0;

	std::cout << "First array" << std::endl;
	for (int i = 0; i < 100; i++) {
		hundredArray[arrayValue] = i;
		std::cout << hundredArray[arrayValue] << std::endl ;            // SECOND PROGRAM FOR 2 ARRAYS
	}

	std::cout << "Second array" << std::endl;
	for (int i = 100; i < 200; i++) {
		hundredArray[arrayValue] = i;
		std::cout << hundredArray[arrayValue] << std::endl;
	}
}*/

/*int hundredArray[100];
int arrayValue = 0;
int twoHundredArray[100];
int arrayValue2 = 0;
int j = 200;

int createHundredArray() {
	std::cout << "First array" << std::endl;
	for (int i = 1; i < 101; i++) {
		hundredArray[arrayValue] = i;
		std::cout << hundredArray[arrayValue] << std::endl;
	}
	return 0;
}

int createTwoHundredArray() {
	std::cout << "Second array" << std::endl;
	for (int i = 101; i < 201; i++) {
		hundredArray[arrayValue2] = i;
		std::cout << hundredArray[arrayValue2] << std::endl;       // THIRD PROGRAM FOR MIXED ARRAY AND IN OWN FUNCTIONS
	}
	return 0;
}

int createMixedHundredArray() {
	std::cout << "Third array" << std::endl;
	for (int i = 1; i < 101; i++) {
		hundredArray[arrayValue] = i;
		twoHundredArray[arrayValue2] = j;
		j--;
		std::cout << hundredArray[arrayValue] << std::endl;
		std::cout << twoHundredArray[arrayValue2] << std::endl;
	}
	return 0;
}

int main()
{
	createHundredArray();
	createTwoHundredArray();
	createMixedHundredArray();
}*/

/*#include<iostream>

using namespace std;

int main()
{
	int i, j, temp, pass = 0;
	int a[10] = { 56,20,1,21,90,101,200,341,249,1 };
	cout << "Input list ...\n";
	for (i = 0; i < 10; i++) {
		cout << a[i] << endl ;
	}

	for (i = 0; i < 10; i++) {
		for (j = i + 1; j < 10; j++){
			if (a[j] < a[i]) {
				temp = a[i];						//	BUBBLE SORT FROM ONLINE
				a[i] = a[j];
				a[j] = temp;
			}
		}
		pass++;
	}
	cout << "\nSorted Element List ...\n";
	for (i = 0; i < 10; i++) {
		cout << a[i] << "\t";
	}
	cout << "\n\nNumber of passes taken to sort the list: " << pass << endl;
	return 0;
}*/