//#include <algorithm>
#include "String.h"

int String::count = 0;

String::String(char const* string) {
	count++;
	c_string = new char[max_string_length + 1];
	strncpy_s(c_string, max_string_length + 1, string, max_string_length);
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

String String::operator + (String const& string) const {
	String temp;
	temp.c_string = new char[strlen(c_string) + strlen(string.c_string) + 1];
	strcpy_s(temp.c_string, strlen(c_string) + 1, c_string);
	strcat_s(temp.c_string, strlen(c_string) + strlen(string.c_string) + 1, string.c_string);
	return temp;
}

String::~String() {
	count--;
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
	return c_string[i];
}

char String::operator[](int i) const {
	return c_string[i];
}

String::operator const char* () {
	return c_string;
}