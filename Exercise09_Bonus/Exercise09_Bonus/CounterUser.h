#pragma once
#include "Observer.h"
#include "LimitedCounter.h"
class CounterUser :
	public Observer
{
public:
	CounterUser() {
		lc = new LimitedCounter(0, 5);
		lc->setObserver(this);
	}

	void IncrementBy(int n) {
		while(n--) {
			++(*lc);
			cout << lc->getCount() << endl;
		}
	}
	
	void HandleLimitReached() {
		std::cout << "Limit has been reached." << std::endl;
		lc->reset();
	}
private:
	LimitedCounter* lc;
};

