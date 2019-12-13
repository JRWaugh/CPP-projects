// Exercise11.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <ctime>
class RandGen {
public:
	RandGen(int range = 10): numbers() { 
		m_range = range;
		srand(time(NULL)); 
	}
	int operator()();
private:
	int m_range;
	std::vector<int> numbers;
};


int main()
{
	std::vector<int> numbers(7), numbers2(7), dupes;
	std::ostream_iterator<int> out_it(std::cout, " ");
	RandGen randGen(37);
	std::generate(numbers.begin(), numbers.end(), randGen);
	std::generate(numbers2.begin(), numbers2.end(), randGen);

	std::cout << "First lotto row: ";
	copy(numbers.begin(), numbers.end(), out_it);
	std::cout << "\nSecond lotto row: ";
	std::copy(numbers2.begin(), numbers2.end(), out_it);

	std::sort(numbers.begin(), numbers.end());
	std::sort(numbers2.begin(), numbers2.end());
	std::set_intersection(numbers.begin(), numbers.end(), numbers2.begin(), numbers2.end(), std::back_inserter(dupes));

	int i = 1;
	if (dupes.size()) {
		std::cout << "\nMatching numbers: " << std::endl;
		std::for_each(dupes.begin(), dupes.end(), [&i](int n) {std::cout << "#" << i++ << ": " << n << std::endl;});

		/* Range based loop test
		for (int n : dupes) {
			std::cout << "#" << i++ << ": " << n << std::endl;
		}
		*/
	}
}

int RandGen::operator()() {
	int number;
	do {
		number = rand() % m_range + 1;
	} while (find(numbers.begin(), numbers.end(), number) != numbers.end());
	numbers.push_back(number);
	return number;
}