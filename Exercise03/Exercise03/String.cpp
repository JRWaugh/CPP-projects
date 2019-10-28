#include "String.h"

String::String(char const *string) {
	c_string = new char[strlen(string) + 1];
	strcpy_s(c_string, strlen(string) + 1, string);
}

String::String(String const &s) { 
	c_string = new char[strlen(s.c_string) + 1];
	strcpy_s(c_string, strlen(s.c_string) + 1, s.c_string);
}

String::~String() {
	delete[] c_string;
}

void String::list() {
	std::cout << c_string << std::endl;
}