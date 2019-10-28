#include <iostream>
#include <sstream>
#include <limits>
#include <iomanip>

class Time {
private:
	int minutes;
	int hours;
public:
	void read(std::string const prompt);
	bool lessThan(Time const &time);
	Time subtract(Time const &time);
	void display();

};

void Time::read(std::string const prompt) {
	std::string str;
	char colon;
	do {
		std::cout << prompt << std::endl;
		std::getline(std::cin, str); //getline so all input up to a linefeed will be read into the string
		if (!(std::stringstream(str) >> hours >> colon >> minutes) || colon != ':') {
			std::cout << "Invalid input." << std::endl;
			colon = 0; //reusing this variable to make the loop easier to work with
		}
	} while (!colon);
}

bool Time::lessThan(Time const &time) {
	if (hours < time.hours || (hours == time.hours && minutes < time.minutes))
		return true;
	else
		return false;
}

Time Time::subtract(Time const &time) {
	Time diff;
	diff.minutes = (hours * 60 + minutes) - (time.hours * 60 + time.minutes);
	diff.hours = diff.minutes / 60;
	diff.minutes %= 60;
	return diff;
}

void Time::display() {
	std::cout << std::setfill('0') << std::setw(2) << hours << ":" << std::setfill('0') << std::setw(2) << minutes << std::endl;
}

