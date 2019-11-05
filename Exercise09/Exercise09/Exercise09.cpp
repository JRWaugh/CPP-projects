#include <iostream>
#include "LimitedCounter.h"
#include "RoundUpCounter.h"

using namespace std;
void UseCounter(Counter* counter, int times);
int main() {
	LimitedCounter counterA(0, 5);
	RoundUpCounter counterB(0, 5);
	UseCounter(&counterA, 8); 
	UseCounter(&counterB, 8);
	cout << "Counter A: " << counterA << endl; // output should be 5
	cout << "Counter B: " << counterB << endl; // output should be 2
	return 0;
}

void UseCounter(Counter* counter, int times) {
	for (int i = 0; i < times; i++) {
		(*counter)++;
	}
}