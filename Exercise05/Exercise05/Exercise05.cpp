#include <iostream>
#include "String.h"

/*void main() {
	String s;
	char name[30];
	s = "Matti"; //Conversion c-string -> String
	std::cout << s;
	strcpy_s(name, 30, s); //Conversion String -> c-string
	std::cout << name;
}*/

// Exercise 5

#include <iostream>
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
	string open_tag = "<" + tag + ">", close_tag = "</" + tag + ">";
	return (text.find(open_tag) != string::npos && text.find(close_tag) != string::npos) ? text.substr(text.find(open_tag) + (open_tag).length(), text.find(close_tag) - text.find(open_tag) - (open_tag).length()) : "Not found!";
}