#include <iostream>
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
	std::cout << prompt << std::endl;
	std::cout << "Enter hours: " << std::endl;
	while (!(std::cin >> hours) || hours < 0 || hours > 23) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Invalid input.\nEnter hours: " << std::endl;
	}

	std::cout << "Enter minutes: " << std::endl;
	while (!(std::cin >> minutes) || minutes < 0 || minutes > 59) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Invalid input.\nEnter minutes: " << std::endl;
	};
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

