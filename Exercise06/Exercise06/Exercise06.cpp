#include <iostream>
#include "String.h"

/*void display(const String &p);

int main(void) {
	String s("abcdefgsxxxxxxx"); // MAX_STRING_LENGTH is 10
	s[2] = 'Y';
	display(s); // outputs the string to the screen
	return 0;
}

void display(const String& p) {
	std::cout << p << std::endl;
}*/

int main() {
	std::cout << String::count << std::endl;
	{
		String array[4];
		std::cout << String::count << std::endl;
	}
	std::cout << String::count << std::endl;
	return 0;
}