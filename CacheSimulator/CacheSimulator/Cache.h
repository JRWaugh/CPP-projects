#ifndef MEMORY_H
#define MEMORY_H
#include "Memory.h"
#endif
#include <memory>
#include <iostream>
#include <iomanip>
#include <vector>
#include <random>

using namespace std;

enum class Policy { FIFO = 1, LRU, Random };

class Cache : public Memory {
	static uint32_c mDirtyBit = 1U << 30;
	static uint32_c mValidBit = 1U << 31;
	friend ostream& operator<<(ostream& os, const Cache& c);

private:
	/*****	 Cache Set Config	 *****/
	/* LRU | 0 | 1 | 2 |...| N | MRU */
	vector<vector<unsigned int>> mSets;

	// Cache parameters
	unsigned int mBlockSize, mSetSize, mTotalSize, mSetCount;
	Policy mPolicy;

	// Cache statistics
	unsigned int mStoreHit, mStoreMiss, mLoadHit, mLoadMiss, mDirtyEvict;

	// Pointer to the next cache level or main memory
	shared_ptr<Memory> mLowerMem;

	// Random generator for Random Replacement strategy. Dis rolls between 0 and set size - 1 inclusive.
	mt19937 gen;

public:
	Cache(uint32_c blockSize, uint32_c setSize, uint32_c totalSize, Policy policy, uint32_c accessTime);

	// The read and write member functions are largely identical but have been kept separate so as not to oversimplify things.
	uint32_c readAddress(uint32_c address);
	uint32_c writeAddress(uint32_c address);
	void setLowerMem(const shared_ptr<Memory> lowerMem) {
		mLowerMem = lowerMem;
	}
	void resetCache() {
		/* Used for clearing out the cache without changing any of the parameters. 
		 * Essentially makes the valid bit useless but it was included anyway.
		 */
		for (auto& v : mSets)
			v.clear();
		mStoreHit = 0, mStoreMiss = 0, mLoadHit = 0, mLoadMiss = 0;
	}

	const double getAMAT() const {
		return mAccessTime + ( (mLoadMiss + mStoreMiss) / ((double)mLoadMiss + mLoadHit + mStoreMiss + mStoreHit) ) * mLowerMem->getAMAT();
	}
};

