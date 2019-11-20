#include "CounterUser.h"
void main() {
	CounterUser* c = new CounterUser();
	c->IncrementBy(12);
	delete c;
} //OUTPUT: two times "Limit has been reached"