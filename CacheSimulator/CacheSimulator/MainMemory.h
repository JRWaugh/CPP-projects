#ifndef MEMORY_H
#define MEMORY_H
#include "Memory.h"
#endif
class MainMemory :
	public Memory
{
public:
	MainMemory(uint32_c accessTime = 400) {
		mAccessTime = accessTime;
	};
	void setAccessTime(uint32_c accessTime) {
		mAccessTime = accessTime;
	};

	uint32_c readAddress(uint32_c address);
	uint32_c writeAddress(uint32_c address);
	const double getAMAT() const {
		return mAccessTime;
	}
};

