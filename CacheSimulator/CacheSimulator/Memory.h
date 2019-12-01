#pragma once
class Memory
{
public:
	virtual unsigned int readAddress(unsigned int address) = 0;
	virtual unsigned int writeAddress(unsigned int address) = 0;
	//virtual ~Memory() {}
protected:
	unsigned int mAccessTime;
};

