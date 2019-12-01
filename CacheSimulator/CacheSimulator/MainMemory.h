#ifndef MEMORY_H
#define MEMORY_H
#include "Memory.h"
#endif
class MainMemory :
	public Memory
{
public:
	MainMemory(unsigned int accessTime = 400) {
		mAccessTime = accessTime;
	};
	void setAccessTime(unsigned int accessTime) {
		mAccessTime = accessTime;
	}
	unsigned int readAddress(unsigned int address);
	unsigned int writeAddress(unsigned int address);
};

