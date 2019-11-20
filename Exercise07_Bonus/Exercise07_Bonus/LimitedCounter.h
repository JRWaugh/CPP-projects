#include "C:\Users\User\Projects\CPPProjects\Exercise07\Exercise07\counter.h"
class LimitedCounter
{
private:
	int m_limit;
	Counter *m_counter;

public:
	LimitedCounter(int n0 = 0, int limit = 0) {
		m_counter = new Counter(n0);
		m_limit = limit;
	}

	LimitedCounter(const LimitedCounter& lc) {
		m_counter = new Counter(*lc.m_counter);
		m_limit = lc.m_limit;
	}

	~LimitedCounter() {
		delete m_counter;
	}

	const LimitedCounter& operator = (const LimitedCounter& lc) {
		if (this != &lc) {
			delete m_counter;
			m_counter = new Counter(lc.m_limit);
		}
		return *this;
	}

	int getCount() const {
		return m_counter->getCount();
	}
	void setLimit(int const limit) {
		m_limit = limit;
	}
	void reset() {
		m_counter->reset();
	}

	LimitedCounter& operator++() {
		if (m_counter->getCount() < m_limit)
			++(*m_counter);
		return *this;
	}

	LimitedCounter operator++(int) {
		LimitedCounter old(*this);
		if (m_counter->getCount() < m_limit)
			++(*m_counter);
		return old;
	}

	bool operator<(int const x) const {
		return m_counter->getCount() < x;
	}

	friend ostream& operator<<(ostream& out, const LimitedCounter& lc) {
		out << lc.m_counter->getCount();
		return out;
	}
};

