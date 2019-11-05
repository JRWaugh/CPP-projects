#include <iostream>
#include <sstream>

using namespace std;

int main()
{
	string str = "11 22 33";
	stringstream ss;

	/* Storing the whole string into string stream */
	ss << str;

	/* Running loop till the end of the stream */
	string temp;
	int found;
	while (!ss.eof()) {

		/* extracting word by word from stream */

		/* Checking the given word is integer or not */
		if (ss >> found)
			cout << found << " ";

		/* To save from space at the end of string */
		temp = "";
	}
}
