class Counter {
private:
	int count;
public:
	void reset();
	void increment();
	int getCount();
};

void Counter::reset() {
	count = 0;
}

void Counter::increment() {
	count++;
}

int Counter::getCount() {
	return count;
}