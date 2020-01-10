#pragma once
typedef unsigned int const uint32_c;

class Memory {
public:
	virtual uint32_c readAddress(uint32_c address) = 0;
	virtual uint32_c writeAddress(uint32_c address) = 0;
	virtual const double getAMAT() const = 0;
	virtual ~Memory() {}
protected:
	unsigned int mAccessTime = 0;
};

