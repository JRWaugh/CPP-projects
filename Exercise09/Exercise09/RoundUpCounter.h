#ifndef COUNTER_H
#define COUNTER_H 
#include "counter.h"
#endif
class RoundUpCounter : public Counter
{
private:
	int m_limit;
public:
	RoundUpCounter(int n0 = 0, int limit = 0) : Counter(n0) {
		m_limit = limit;
	}
	RoundUpCounter& operator++() {
		Counter::operator++();
		if (getCount() == m_limit) {
			reset();
		}
		return *this;
	}

	Counter operator++(int) {
		RoundUpCounter old(*this);
		Counter::operator++();
		if (getCount() == m_limit) {
			reset();
		}
		return old;
	}
};

