#include <iostream>
#include <cstdlib>
#include <ctime>  

int mean(int* array, int n);
int main()
{
	int amount;
	std::cout << "How many numbers are needed?\n";
	std::cin >> amount;
	int* numbers = new int[amount];

	srand(time(NULL));
	for (int i = 0; i < amount; i++) {
		numbers[i] = rand() % 100;
		std::cout << numbers[i] << "\n";
	}

	std::cout << "The mean is " << mean(numbers, amount) << "\n";
	delete[] numbers;
}


int mean(int* array, int n) {
	int total = 0;
	for (int i = 0; i < n; i++) {
		total += array[i];
	}
	return total / n;
}