#include <iostream>
#include <sstream>
#include <string>
#include <vector>
int main()
{
	std::vector<std::vector<int>> vect_2d;
	std::stringstream ss;
	std::string input;
	int cols, rows, val;
	std::cout << "Enter number of rows: " << std::endl;
	std::cin >> rows;
	std::cout << "Enter number of columns: " << std::endl;
	std::cin >> cols;
	
	vect_2d.resize(rows, std::vector<int>(cols, 0));

	for (auto& row : vect_2d) {
		std::cout << "Row: ";
		for (auto& col : row) {
			std::cin >> col;
		}
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << std::endl;
	}

	for (auto& row : vect_2d) {
		for (auto& col : row) {
			std::cout << col << " ";
		}
		std::cout << std::endl;
	}
}