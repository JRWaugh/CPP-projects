#include <iostream>
#include "String.h"

void main() {
	String s;
	char name[30];
	s = "Matti"; //Conversion c-string -> String
	std::cout << s;
	strcpy_s(name, 30, s); //Conversion String -> c-string
	std::cout << name;
}

// Exercise 5

/*#include <iostream>
#include <fstream>
#include <string>

using namespace std;
string tag_search(string const &text, string const &tag);

int main() {
	string page, line, location, temperature;
	int open_tag = 0, close_tag = 0;
	ifstream inputFile("weather.xml");

	while (getline(inputFile, line)) {
		page.append(line);
		line.erase();
	}
	location = tag_search(page, "location");
	temperature = tag_search(page, "temp_c");

	cout << "Location: " << location << endl;
	cout << "Temperature: " << temperature << endl;
	system("pause");
}

string tag_search(string const &text, string const &tag) {
	return text.substr(text.find("<" + tag + ">") + ("<" + tag + ">").length(), text.find("</" + tag + ">") - text.find("<" + tag + ">") - ("<" + tag + ">").length());
}*/