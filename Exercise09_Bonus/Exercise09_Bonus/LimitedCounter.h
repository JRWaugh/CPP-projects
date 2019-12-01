#ifndef COUNTER_H
#define COUNTER_H 
#include "counter.h"
#endif
class LimitedCounter : public Counter
{
private:
	int m_limit;
	Observer* m_obs;
public:
	LimitedCounter(int n0 = 0, int limit = 0) : Counter(n0) {
		m_limit = limit;
		m_obs = nullptr;
	}

	void Notify() {
		if(m_obs)
			m_obs->HandleLimitReached();
	}

	LimitedCounter& operator++() {
		Counter::operator++();
		if (getCount() == m_limit)
			Notify();
		return *this;
	}

	Counter operator++(int) {
		LimitedCounter old(*this);
		Counter::operator++();
		if (getCount() == m_limit)
			Notify();
		return old;
	}

	void setObserver(Observer* obs) {
		m_obs = obs;
	}
};

