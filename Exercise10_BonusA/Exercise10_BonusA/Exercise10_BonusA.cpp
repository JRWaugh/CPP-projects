// Exercise10_BonusA.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
int main()
{
	std::vector<std::vector<int>> vect_2d;
	std::string input;
	int col, row, val;
	std::cout << "Enter number of rows: " << std::endl;
	std::cin >> row;
	std::cout << "Enter number of columns: " << std::endl;
	std::cin >> col;
	
	vect_2d.resize(row, std::vector<int>(col, 0));

	for (int i = 0; i < row; ++i) {
		std::cout << "Row number " << i + 1 << std::endl;
		for (int j = 0; j < col; ++j) {
			std::getline(std::cin, input); 
			ss. >> vect_2d[j][i];

		}
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
