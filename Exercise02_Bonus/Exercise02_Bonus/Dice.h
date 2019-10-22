#include <cstdlib>
#include <ctime> 

class Dice {
public:
	void initialize();
	float roll();
};

void Dice::initialize() {
	srand(time(NULL));
}

float Dice::roll() {
	return (float)rand() / RAND_MAX;
}