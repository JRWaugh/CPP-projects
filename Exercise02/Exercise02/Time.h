#include <iostream>
#include <sstream>
#include <limits>
#include <iomanip>

class Time {
private:
	int minutes;
	int hours;
public:
	void read(std::string prompt);
	bool lessThan(Time time);
	Time subtract(Time time);
	void display();

};

void Time::read(std::string prompt) {
	//Only verifying that positive numbers are given. Will reduce large numbers down to sensible numbers if necessary.
	std::string str;

	do {
		std::cout << prompt << std::endl;
		std::getline(std::cin, str);
		if (str.find(':') != -1) {
			str[str.find(':')] = ' ';
			std::stringstream(str) >> hours >> minutes;
		}
		else
			hours = -1;
	} while (hours < 0 || hours > 23 || minutes < 0 || minutes > 59);
}

bool Time::lessThan(Time time) {
	if (hours < time.hours || (hours == time.hours && minutes < time.minutes))
		return true;
	else
		return false;
}

Time Time::subtract(Time time) {
	Time diff;
	diff.minutes = (hours * 60 + minutes) - (time.hours * 60 + time.minutes);
	diff.hours = diff.minutes / 60;
	diff.minutes %= 60;
	return diff;
}

void Time::display() {
	std::cout << std::setfill('0') << std::setw(2) << hours << ":" << std::setfill('0') << std::setw(2) << minutes << std::endl;
}

