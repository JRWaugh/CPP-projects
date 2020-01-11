#include "Cache.h"

Cache::Cache(uintc32_t blockSize = 0, uintc32_t setSize = 0, uintc32_t totalSize = 0, Policy policy = Policy::LRU, uintc32_t accessTime = 0, uintc32_t accessTimeLower = 0) :
	mBlockSize{ 1U << blockSize }, mSetSize{ 1U << setSize }, mTotalSize{ 1U << totalSize }, mSetCount{ mTotalSize / (mBlockSize * mSetSize) }, mPolicy{ policy },
	mStoreHit{ 0 }, mStoreMiss{ 0 }, mLoadHit{ 0 }, mLoadMiss{ 0 }, mDirtyEvict{ 0 },
	mLowerMem { make_shared<MainMemory>(accessTimeLower) },
	gen{ random_device{}() }
{
	mAccessTime = accessTime;
	mSets.resize(mSetCount);
};

uintc32_t Cache::readAddress(uintc32_t address) {
	uintc32_t blockNumber = address / mBlockSize;
	uintc32_t index = blockNumber % mSetCount;
	uintc32_t tag = blockNumber / mSetCount;
	unsigned int cycles = mAccessTime;

	// This algorithm searches through the index for the desired tag.
	auto tagIter = find_if(mSets[index].begin(), mSets[index].end(), [&](auto block) {
		return (block.first & mValidBit) ? tag == block.second / mBlockSize : false;
		});

	if (tagIter == mSets[index].end()) { // The tag was not found...
		mLoadMiss++;

		if (mSets[index].size() == mSetSize) { // ... and the set is full
			if (mPolicy == Policy::Random)
				shuffle(mSets[index].begin(), mSets[index].end(), gen);
			else // FIFO or LRU rotates everything 1 to the left so that the front element is now the back element
				rotate(mSets[index].begin(), next(mSets[index].begin()), mSets[index].end());

			if (mSets[index].back().first & mDirtyBit) { // ... and the back element has the dirty bit set
				mDirtyEvict++;
				/* The sim can have differently sized blocks (eg. L2 smaller than L1) so reconstructing
				 * the address this way ensures everything behaves "properly". 
				 */
				auto reconstructedAddress = ((mSets[index].back().second / mBlockSize) * mSetCount * mBlockSize | (index * mBlockSize) | (mSets[index].back().second & (mBlockSize - 1)));
				cycles += mLowerMem->writeAddress(reconstructedAddress);
			}
			mSets[index].back() = make_pair(mValidBit, (tag * mBlockSize) | (address & (mBlockSize - 1)));
		}
		else
			mSets[index].push_back(make_pair(mValidBit, (tag * mBlockSize) | (address & (mBlockSize - 1))));

		cycles += mLowerMem->readAddress(address);			
	}
	else {
		mLoadHit++;
		if (mPolicy == Policy::LRU && mSets[index].back() != *tagIter)
			// Moves accessed block to the back of the set (MRU) while maintaining the order of the other blocks.
			rotate(tagIter, next(tagIter), mSets[index].end());
	}
	return cycles;
}

uintc32_t Cache::writeAddress(uintc32_t address) {
	int blockNumber = address / mBlockSize;
	int index = blockNumber % mSetCount; 
	int tag = blockNumber / mSetCount;
	int cycles = mAccessTime;

	auto tagIter = find_if(mSets[index].begin(), mSets[index].end(), [&](auto block) {
		return (block.first & mValidBit) ? tag == block.second / mBlockSize : false;
		});

	if (tagIter == mSets[index].end()) {
		mStoreMiss++;

		if (mSets[index].size() == mSetSize) {
			if (mPolicy == Policy::Random)
				shuffle(mSets[index].begin(), mSets[index].end(), gen);
			else
				rotate(mSets[index].begin(), next(mSets[index].begin()), mSets[index].end());

			if (mSets[index].back().first & mDirtyBit) {
				mDirtyEvict++;
				auto reconstructedAddress = ( (mSets[index].back().second / mBlockSize) * mSetCount * mBlockSize | (index * mBlockSize) | (mSets[index].back().second & (mBlockSize - 1)) );
				cycles += mLowerMem->writeAddress(reconstructedAddress);
			}
			mSets[index].back() = make_pair(mValidBit | mDirtyBit, (tag * mBlockSize) | (address & (mBlockSize - 1)));
		}
		else
			mSets[index].push_back(make_pair(mValidBit | mDirtyBit, (tag * mBlockSize) | (address & (mBlockSize - 1))));

		cycles += mLowerMem->readAddress(address);
	}
	else {
		mStoreHit++;
		*tagIter = make_pair(mValidBit | mDirtyBit, (tag * mBlockSize) | (address & (mBlockSize - 1)));
		if (mPolicy == Policy::LRU && mSets[index].back() != *tagIter)
			rotate(tagIter, next(tagIter), mSets[index].end());
	}
	return cycles;
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