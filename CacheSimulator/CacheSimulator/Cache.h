#ifndef MEMORY_H
#define MEMORY_H
#include "Memory.h"
#endif
//#include "MainMemory.h"
#include <memory>
#include <iostream>
#include <list>
#include <vector>
class Cache :
	public Memory
{
public:
	Cache(unsigned int blockSize = 0, unsigned int setSize = 0, unsigned int totalSize = 0, unsigned int accessTime = 0) :
		mBlockSize{ 1U << blockSize },
		mSetSize { setSize },
		mTotalSize{ 1U << totalSize },
		mMissCount { 0 },
		mHitCount { 0 }
	{
		mAccessTime = accessTime;
		mSetCount = mTotalSize / (mBlockSize * mSetSize);
		mIndex.resize(mSetCount, std::list<int>(0, 0)); 
		mLowerMem = nullptr;
	};
	unsigned int readAddress(unsigned int address);
	unsigned int writeAddress(unsigned int address);
	void setLowerMem(std::shared_ptr<Memory> lowerMem) {
		mLowerMem = lowerMem;
	}
	unsigned int getHitCount() {
		return mHitCount;
	}
	unsigned int getMissCount() {
		return mMissCount;
	}

	void resetCache() {
		//Used for clearing out the cache without changing any of the parameters
		mIndex.resize(mSetCount, std::list<int>(0, 0));
		mHitCount = 0;
		mMissCount = 0;
	}

private:
	unsigned int mBlockSize; //offset. some power of 2.
	unsigned int mSetSize;
	unsigned int mTotalSize;
	unsigned int mSetCount; //same as block count if setSize is 1.
	unsigned int mHitCount;
	unsigned int mMissCount;
	static unsigned int const mDirtyBit = 1U << 30; //applied to blocks
	//static unsigned int const mValidBi = 1U << 31; might not even use this but it's here for now
	std::shared_ptr<Memory> mLowerMem;
	std::vector<std::list<int>> mIndex;
};

