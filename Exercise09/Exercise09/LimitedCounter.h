#ifndef COUNTER_H
#define COUNTER_H 
#include "counter.h"
#endif
class LimitedCounter : public Counter
{
private:
	int m_limit;
public:
	LimitedCounter(int n0 = 0, int limit = 0) : Counter(n0) {
		m_limit = limit;
	}
	LimitedCounter& operator++() {
		if (getCount() < m_limit)
			Counter::operator++();
		return *this;
	}

	Counter operator++(int) {
		if (getCount() < m_limit) {
			LimitedCounter old(*this);
			Counter::operator++();
			return old;
		}
		else
			return *this;
	}
};

