#include "Cache.h"

Cache::Cache(uint32_c blockSize = 0, uint32_c setSize = 0, uint32_c totalSize = 0, Policy policy = Policy::LRU, uint32_c accessTime = 0) :
	mBlockSize{ 1U << blockSize }, mSetSize{ 1U << setSize }, mTotalSize{ 1U << totalSize },
	mDirtyEvict{ 0 }, mStoreHit{ 0 }, mStoreMiss{ 0 }, mLoadHit{ 0 }, mLoadMiss{ 0 },
	mPolicy{ policy },
	gen{ random_device{}() }
{
	mAccessTime = accessTime;
	mSetCount = mTotalSize / (mBlockSize * mSetSize);
	mSets.resize(mSetCount, vector<unsigned int>(0, 0));
	mLowerMem = nullptr;
};

uint32_c Cache::readAddress(uint32_c address) {
	uint32_c blockNumber = address / mBlockSize;
	uint32_c index = blockNumber % mSetCount;
	uint32_c tag = blockNumber / mSetCount;
	unsigned int cycles = 0;

	// This algorithm searches through the index for the desired tag.
	auto tagIter = find_if(mSets[index].begin(), mSets[index].end(), [&](auto block) {
		return (block & mValidBit) ? tag == (block & ~(mValidBit | mDirtyBit)) / mBlockSize : false;
		});

	if (tagIter == mSets[index].end()) { // The tag was not found...
		mLoadMiss++;

		if (mSets[index].size() == mSetSize) { // ... and the set is full
			if (mPolicy == Policy::Random)
				shuffle(mSets[index].begin(), mSets[index].end(), gen);
			else // FIFO or LRU rotates everything 1 to the left so that the front element is now the back element
				rotate(mSets[index].begin(), next(mSets[index].begin()), mSets[index].end());

			if (mSets[index].back() & mDirtyBit) { // ... and the back element has the dirty bit set
				mDirtyEvict++;
				/* The sim can have differently sized blocks (eg. L2 smaller than L1) so reconstructing
				 * the address this way ensures everything behaves "properly". 
				 */
				cycles += mLowerMem->writeAddress((mSets[index].back() * mSetCount) | (index * mBlockSize) | (mSets[index].back() & (mBlockSize - 1)));
			}
			mSets[index].back()  =  mValidBit | (tag * mBlockSize) | (address & (mBlockSize - 1));
		}
		else
			mSets[index].push_back( mValidBit | (tag * mBlockSize) | (address & (mBlockSize - 1)) );

		cycles += mLowerMem->readAddress(address);			
	}
	else {
		mLoadHit++;
		if (mPolicy == Policy::LRU && mSets[index].back() != *tagIter)
			// Moves accessed block to the back of the set (MRU) while maintaining the order of the other blocks.
			rotate(tagIter, next(tagIter), mSets[index].end());
	}
	return mAccessTime + cycles;
}

uint32_c Cache::writeAddress(uint32_c address) {
	int blockNumber = address / mBlockSize;
	int index = blockNumber % mSetCount; 
	int tag = blockNumber / mSetCount;
	int cycles = 0;

	auto tagIter = find_if(mSets[index].begin(), mSets[index].end(), [&](auto block) {
		return (block & mValidBit) ? tag == (block & ~(mValidBit | mDirtyBit)) / mBlockSize : false;
		});

	if (tagIter == mSets[index].end()) {
		mStoreMiss++;

		if (mSets[index].size() == mSetSize) {
			if (mPolicy == Policy::Random)
				shuffle(mSets[index].begin(), mSets[index].end(), gen);
			else
				rotate(mSets[index].begin(), next(mSets[index].begin()), mSets[index].end());

			if (mSets[index].back() & mDirtyBit) {
				mDirtyEvict++;
				cycles += mLowerMem->writeAddress((mSets[index].back() * mSetCount) | (index * mBlockSize) | (mSets[index].back() & (mBlockSize - 1)));
			}
			mSets[index].back()  =  mValidBit | mDirtyBit | (tag * mBlockSize) | (address & (mBlockSize - 1));
		}
		else
			mSets[index].push_back( mValidBit | mDirtyBit | (tag * mBlockSize) | (address & (mBlockSize - 1)) );

		cycles += mLowerMem->readAddress(address);
	}
	else {
		mStoreHit++;
		*tagIter = mValidBit | mDirtyBit | (tag * mBlockSize) | (address & (mBlockSize - 1));
		if (mPolicy == Policy::LRU && mSets[index].back() != *tagIter)
			rotate(tagIter, next(tagIter), mSets[index].end());
	}
	return mAccessTime + cycles;
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