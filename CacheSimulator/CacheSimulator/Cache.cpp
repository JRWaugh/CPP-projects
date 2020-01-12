#include "Cache.h"

Cache::Cache(uintc32_t blockSize = 0, uintc32_t setSize = 0, uintc32_t totalSize = 0, Policy policy = Policy::LRU, uintc32_t accessTime = 0, uintc32_t accessTimeLower = 0) :
	mBlockSize{ 1U << blockSize }, mSetSize{ 1U << setSize }, mTotalSize{ 1U << totalSize }, mSetCount{ mTotalSize / (mBlockSize * mSetSize) }, mPolicy{ policy },
	mStoreHit{ 0 }, mStoreMiss{ 0 }, mLoadHit{ 0 }, mLoadMiss{ 0 }, mDirtyEvict{ 0 },
	mLowerMem { make_shared<MainMemory>(accessTimeLower) }, gen{ random_device{}() }
{
	mAccessTime = accessTime;
	mSets.resize(mSetCount);
};

std::optional<uintc32_t> Cache::accessAddress(uintc32_t address, const unsigned char instruction) {
	/* This function will return an option containing either a value (cycles) or a nullopt.
	 * This was an exercise in rudimentary error handling and isn't all that useful.
	 */
	uintc32_t blockNumber = address / mBlockSize;
	uintc32_t index = blockNumber % mSetCount;
	uintc32_t tag = blockNumber / mSetCount;
	unsigned int cycles = mAccessTime;
	std::optional<unsigned int> result;

	/* This algorithm searches through the index for the desired tag. */
	auto tagIter = find_if(mSets[index].begin(), mSets[index].end(), [&](auto block) {
		return (block.first & mValidBit) ? tag == block.second / mBlockSize : false;
		});

	if (tagIter == mSets[index].end()) { // The tag was not found...
		if (instruction == 'l')
			++mLoadMiss;
		else if (instruction == 's')
			++mStoreMiss;
		else
			return nullopt;

		if (mSets[index].size() == mSetSize) { // ... and the set is full
			if (mPolicy == Policy::Random)
				shuffle(mSets[index].begin(), mSets[index].end(), gen);
			else 
				/* FIFO or LRU rotates everything 1 to the left so that the front (LRU) element is at the back so it can be removed. */
				rotate(mSets[index].begin(), next(mSets[index].begin()), mSets[index].end());

			if (mSets[index].back().first & mDirtyBit) { // ... and the back element has the dirty bit set
				++mDirtyEvict;
				/* The sim can have differently sized blocks (eg. L2 smaller than L1) so reconstructing
				 * the address this way ensures everything behaves "properly". 
				 */
				auto reconstructedAddress = ((mSets[index].back().second / mBlockSize) * mSetCount * mBlockSize | (index * mBlockSize) | (mSets[index].back().second & (mBlockSize - 1)));
				cycles += mLowerMem->accessAddress(reconstructedAddress, 's').value();
			}
			/* The LSB of 'l' is 0 and the LSB of 's' is 1, so the bitwise & will set the dirty bit if the instruction is a store. */
			mSets[index].back() = make_pair(mValidBit | (instruction & mDirtyBit), (tag * mBlockSize) | (address & (mBlockSize - 1)));
		}
		else 
			mSets[index].push_back(make_pair(mValidBit | (instruction & mDirtyBit), (tag * mBlockSize) | (address & (mBlockSize - 1))));

		cycles += mLowerMem->accessAddress(address, 'l').value();			
	}
	else {
		if (instruction == 'l')
			++mLoadHit;
		else if (instruction == 's') {
			++mStoreHit;
			*tagIter = make_pair(mValidBit | mDirtyBit, (tag * mBlockSize) | (address & (mBlockSize - 1)));
		}
		else
			return nullopt;

		if (mPolicy == Policy::LRU && mSets[index].back() != *tagIter)
			/* Moves accessed block to the back of the set (MRU) while maintaining the order of the other blocks. */
			rotate(tagIter, next(tagIter), mSets[index].end());
	}
	return cycles;
}

void Cache::setLowerMem(const shared_ptr<MainMemory> lowerMem) {
	mLowerMem = lowerMem;
}

void Cache::invalidateCache() {
	/* Unsets the valid bits in each block in each set of the cache. Should be called when a new file is read. */
	for (auto& set : mSets)
		for (auto& block : set)
			block.first &= ~mValidBit;
}

void Cache::resetCacheStats() {
	/* Used for clearing out cache statistics without changing any of the parameters. */
	mDirtyEvict = mStoreHit = mStoreMiss = mLoadHit = mLoadMiss = 0;
}

const double Cache::getAMAT() const {
	return (double)mAccessTime + (((double)mLoadMiss + mStoreMiss) / ((double)mLoadMiss + mLoadHit + mStoreMiss + mStoreHit)) * mLowerMem->getAMAT();
}

ostream& operator<<(ostream& os, const Cache& c) {
	return os << 
		left << setw(20) << "Load hit count:"   << right << setw(10) << c.mLoadHit    << "\t" << 
		left << setw(20) << "Load miss count:"  << right << setw(10) << c.mLoadMiss   << endl <<
		left << setw(20) << "Store hit count:"  << right << setw(10) << c.mStoreHit   << "\t" << 
		left << setw(20) << "Store miss count:" << right << setw(10) << c.mStoreMiss  << endl <<
		left << setw(20) << "Dirty evictions:"  << right << setw(10) << c.mDirtyEvict << "\t" << 
		left << setw(20) << "Hit ratio:"		<< right << setw(10) <<
		((double)c.mLoadHit + c.mStoreHit) / ((double)c.mLoadHit + c.mLoadMiss + c.mStoreHit + c.mStoreMiss) << endl <<
		left << setw(20) << "Average Access Time:" << right << setw(10) << c.getAMAT();
}