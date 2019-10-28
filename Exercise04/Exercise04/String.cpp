#include <algorithm>
#include "String.h"

String::String(char const *string) {
	c_string = new char[strlen(string) + 1];
	strcpy_s(c_string, strlen(string) + 1, string);
}

String::String(String const &string) {
	c_string = new char[strlen(string.c_string) + 1];
	strcpy_s(c_string, strlen(string.c_string) + 1, string.c_string);
}

String& String::operator = (String string) {
	//Copied mostly from online because I realised I was writing a copy constructor/destructor twice.
	swap(*this, string);
	return *this;
}

String String::operator + (String const &string) const {
	String temp;
	temp.c_string = new char[strlen(c_string) + strlen(string.c_string) + 1];
	strcpy_s(temp.c_string, strlen(c_string) + 1, c_string);
	strcat_s(temp.c_string, strlen(c_string) + strlen(string.c_string) + 1, string.c_string);
	return temp;
}

String::~String() {
	delete[] c_string;
}

void String::list() {
	std::cout << c_string << std::endl;
}

std::ostream& operator << (std::ostream& os, String const &string) {
	os << string.c_string;
	return os;
}