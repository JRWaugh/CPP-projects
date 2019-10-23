#include "String.h"

String::String(const char* string) {
	c_string = new char[strlen(string) + 1];
	strcpy_s(c_string, strlen(string) + 1, string);
}

String::String(const String& s) {
	c_string = new char[strlen(s.c_string) + 1];
	strcpy_s(c_string, strlen(s.c_string) + 1, s.c_string);
}

const String String::operator = (const String &string) {
	if (this != &string) {
		delete c_string;
		c_string = new char[strlen(string.c_string) + 1];
		strcpy_s(c_string, strlen(string.c_string) + 1, string.c_string);
	}
	return *this;
}

String String::operator + (const String &string) const {
	String temp;
	temp.c_string = new char[strlen(c_string) + strlen(string.c_string) + 1];
	strcpy_s(temp.c_string, strlen(c_string) + 1, c_string);
	strcat_s(temp.c_string, strlen(c_string) + strlen(string.c_string) + 1, string.c_string);
	return temp;
}

String::~String() {
	delete c_string;
}

void String::list() {
	std::cout << c_string << std::endl;
}

std::ostream& operator << (std::ostream& os, const String& string) {
	os << string.c_string;
	return os;
}