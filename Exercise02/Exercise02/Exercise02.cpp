#include <iostream>
#include "Time.h"

int main() {
	Time time1, time2, duration;
	time1.read("Enter time 1");
	time2.read("Enter time 2");
	if (time1.lessThan(time2)) {
		duration = time2.subtract(time1);
		std::cout << "Starting time was ";
		time1.display();
	}
	else {
		duration = time1.subtract(time2);
		std::cout << "Starting time was ";
		time2.display();
	}
	std::cout << "Duration was ";
	duration.display();
	return 0;
}
