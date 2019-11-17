#include <iostream>
#include "String.h"

void display(const String &p);

int main(void) {
	String s("abcdefgsxxxxxxx"); // MAX_STRING_LENGTH is 10
	s[2] = 'Y';
	++s;
	s++;
	display(s); // outputs the string to the screen
	std::cout << String::count << std::endl;
	{
		String array[4];
		std::cout << String::count << std::endl;
	}
	std::cout << String::count << std::endl;
	return 0;
}

void display(const String& p) {
	for(int i = 0; p[i]; i++)
		std::cout << p[i];
	std::cout << std::endl;
}

