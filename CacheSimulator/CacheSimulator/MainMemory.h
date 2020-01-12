#ifndef MMEMORY_H
#define MMEMORY_H
#include <optional>
typedef const unsigned int uintc32_t;

class MainMemory {
protected:
	unsigned int mAccessTime = 0;

public:
	MainMemory(uintc32_t accessTime = 400) : mAccessTime{ accessTime } {};
	virtual std::optional<uintc32_t> accessAddress(uintc32_t address, const unsigned char instruction);
	virtual const double getAMAT() const;
};
#endif