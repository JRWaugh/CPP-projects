#include <iostream>
#include "String.h"
int main() {
	String s1("abcedf");
	String s2("ghijklmn");
	String sum;
	std::cout << "String 1 is:" << s1 << std::endl;
	std::cout << "String 2 is:" << s2 << std::endl;
	sum = s1 + s2;
	std::cout << "Sum is :" << sum << std::endl; // output should be
	// abcdefghijklmn
   // This should also work
	sum = sum;
	std::cout << "Sum is still :" << sum << std::endl; // output should
	 // still be abcdefghijklmn
	return 0;
}