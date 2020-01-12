#include "MainMemory.h"

std::optional<uintc32_t> MainMemory::accessAddress(uintc32_t address, const unsigned char instruction) {
	return mAccessTime;
}

const double MainMemory::getAMAT() const {
	return mAccessTime;
}