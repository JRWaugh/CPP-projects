/*
 * circularbuffer.h
 *
 *  Created on: 9 Feb 2020
 *      Author: Joshua
 */
#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

template <typename T, unsigned int N>
class circularbuffer {
public:
	circularbuffer() : buffer{ 0 }, size_{ 0 }, tail{ buffer }, end{ buffer + N } {}

	/* Not sure if this constructor does what I want it to do. */
	circularbuffer(T const array[]) : buffer{ array }, size_{ sizeof(array) }, tail{ buffer }, end{ buffer + N } {}

	template <typename... Type>
	circularbuffer(Type... args) : buffer{ static_cast<T>(args)... }, size_{ sizeof...(Type) }, tail{ buffer }, end{ buffer + N } {}

	unsigned int const size() const {
		return size_;
	}

	T& operator[](unsigned int const index) {
		if (index < N)
			return (tail + index) >= end ? *(tail + index - N) : *(tail + index);
		else
			return *tail; // throw exception or asset an error later
	}

	void push_back(T const t) {
		if (size_ < N)
			this->operator[](size_++) = t;
		else {
			*tail = t;
			if (++tail == end)
				tail = buffer;
		}
	}

	void push_front(T const t) {
		if (--tail < buffer)
			tail = end - 1;
		*tail = t;

		if (size_ < N)
			++size_;
	}

	void pop_back() {
		if (size_ > 0)
			--size_;
	}

	void pop_front() {
		if (size_ > 0) {
			--size_;
			if (++tail == end)
				tail = buffer;
		}
	}

private:
	T buffer[N];
	unsigned int size_;
	T* tail; /* Points to the oldest datum, although that gets muddied if you use push_front() */
	T* end;
};
#endif /* CIRCULARBUFFER_H_ */
