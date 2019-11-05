#include <iostream>
#include "String.h"
int main() {
	String s("abcdefg");
	for (int i = 0; i < 7; i++)
		std::cout << s[i] << " ";
	s[4] = 'X';
	for (int i = 0; i < 7; i++)
		std::cout << s[i] << " ";
	return 0;
}