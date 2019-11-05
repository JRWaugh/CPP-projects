#include "counter.h"
class LimitedCounter
{
private:
	int m_limit;
	Counter m_counter;
public:
	LimitedCounter(int n0 = 0, int limit = 0) : m_counter(n0) {
		m_limit = limit;
	}
	int getCount() const {
		return m_counter.getCount();
	}
	void setLimit(int const limit) {
		m_limit = limit;
	}
	void reset() {
		m_counter.reset();
	}

	LimitedCounter& operator++() {
		if (m_counter.getCount() < m_limit)
			m_counter++;
		return *this;
	}

	LimitedCounter operator++(int) {
		LimitedCounter old(*this);
		if (m_counter.getCount() < m_limit)
			m_counter++;
		return old;
	}

	bool operator<(int const x) const {
		return m_counter.getCount() < x;
	}

	friend ostream& operator<<(ostream& out, const LimitedCounter& lc) {
		out << lc.m_counter.getCount() << std::endl;
		return out;
	}
};

