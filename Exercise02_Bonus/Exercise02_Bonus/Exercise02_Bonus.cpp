#include <iostream>
#include "Dice.h"
#include "Counter.h"
#include "Inspector.h"

int main() {
	Dice dice;
	Counter counter1, counter2, i;
	Inspector inspector;
	dice.initialize();
	counter1.reset(); counter2.reset(), i.reset();
	inspector.setLimits(0.0, 0.5);
	while (i.getCount() < 100) {
		if (inspector.isInLimits(dice.roll()))
			counter1.increment();
		else
			counter2.increment();
		i.increment();
	}
	std::cout << counter1.getCount() << std::endl;
	std::cout << counter2.getCount() << std::endl;
	return 0;
}
