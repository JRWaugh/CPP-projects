#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>
using namespace std;
int main()
{
	/* Part a
	pair<int, pair<string, string>> person1(123, make_pair("aaaaa", "xxxxxxx")), person2(345, make_pair("bbb", "yyy"));
	cout << "no\tfirst name\tlast name" << endl;
	cout << person1.first << "\t" << setw(10) << left << person1.second.first << "\t" << person1.second.second << endl;
	cout << person2.first << "\t" << setw(10) << left << person2.second.first << "\t" << person2.second.second << endl;
	*/

	//Part b
	vector<string> strings;
	vector<string>::iterator it;
	string input, longest = "";
	cout << "Enter text. Enter \"stop\" to end program" << endl;
	while (getline(cin, input) && input != "stop") {
		strings.push_back(input);
		if (input.size() > longest.size())
			longest = input;
	}

	cout << "Longest word was \"" << longest << "\"" << endl << endl;
	cout << "List of all words entered:" << endl;

	for (it = strings.begin(); it != strings.end(); ++it) {
		cout << *it << endl;
	}

	cout << "Number of words entered: " << strings.size() << endl;
	cout << "Size: " << strings.size() << "\t Capacity: " << strings.capacity() << "\t Sizeof: " << sizeof(strings) << endl;
}