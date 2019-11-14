#include <algorithm>
#include "String.h"

String::String(char const* string) {
	c_string = new char[strlen(string) + 1];
	strcpy_s(c_string, strlen(string) + 1, string);
}

String::String(String const& string) {
	c_string = new char[strlen(string.c_string) + 1];
	strcpy_s(c_string, strlen(string.c_string) + 1, string.c_string);
}

const String& String::operator = (const String& string) {
	if (this != &string) {
		delete[] c_string;
		c_string = new char[strlen(string.c_string) + 1];
		strcpy_s(c_string, strlen(string.c_string) + 1, string.c_string);
	}
	return *this;
}

const String& String::operator = (const char* string) {
	delete[] c_string;
	c_string = new char[strlen(string) + 1];
	strcpy_s(c_string, strlen(string) + 1, string);
	return *this;
}

String String::operator + (String const& string) const {
	String temp;
	delete[] c_string;
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

std::ostream& operator << (std::ostream& os, String const& string) {
	os << string.c_string;
	return os;
}

String& String::operator++() {
	char* temp = new char[strlen(c_string) + 2];
	strcpy_s(temp, strlen(c_string) + 2, c_string);
	strcat_s(temp, strlen(c_string) + 2, "X");
	delete[] c_string;
	c_string = temp;
	return *this;
}

String String::operator++(int) {
	String old(*this);
	char* temp = new char[strlen(old.c_string) + 2];
	strcpy_s(temp, strlen(old.c_string) + 2, old.c_string);
	strcat_s(temp, strlen(old.c_string) + 2, "X");
	delete[] c_string;
	c_string = temp;
	return old;
}

char& String::operator[](int i) {
	if (i >= 0 && i < strlen(c_string))
		return c_string[i];
	else {
		std::cout << "Index out of bounds" << std::endl;
		return c_string[0];
	}
}

String::operator const char* () {
	return c_string;
}