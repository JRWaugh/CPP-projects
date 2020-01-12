#pragma once

using uintc32_t = const unsigned int;
constexpr char load = 'l', store = 's';

class MainMemory {
protected:
    unsigned int mAccessTime;

public:
    MainMemory(uintc32_t accessTime = 400) : mAccessTime{ accessTime } {};
    virtual uintc32_t accessAddress(uintc32_t address, const unsigned char instruction) {
        return mAccessTime;
    }
    virtual const double getAMAT() const {
        return mAccessTime;
    }
};
