#ifndef MMEMORY_H
#define MMEMORY_H
typedef const unsigned int uintc32_t;

class MainMemory {
protected:
	unsigned int mAccessTime = 0;

public:
	MainMemory(uintc32_t accessTime = 400) {
		mAccessTime = accessTime;
	};

	virtual uintc32_t readAddress(uintc32_t address);
	virtual uintc32_t writeAddress(uintc32_t address);
	virtual const double getAMAT() const {
		return mAccessTime;
	}
};
#endif