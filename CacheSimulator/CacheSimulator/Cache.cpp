#include "Cache.h"

Cache::Cache(uint32_c blockSize = 0, uint32_c setSize = 0, uint32_c totalSize = 0, Policy policy = Policy::LRU, uint32_c accessTime = 0) :
	mBlockSize{ 1U << blockSize }, mSetSize{ setSize }, mTotalSize{ 1U << totalSize },
	mDirtyEvict{ 0 }, mStoreHit{ 0 }, mStoreMiss{ 0 }, mLoadHit{ 0 }, mLoadMiss{ 0 },
	mPolicy{ policy },
	gen{ std::random_device{}() },
{
	mAccessTime = accessTime;
	mSetCount = mTotalSize / (mBlockSize * mSetSize);
	mSets.resize(mSetCount, std::vector<unsigned int>(0, 0));
	mLowerMem = nullptr;
};

uint32_c Cache::readAddress(uint32_c address) {
	uint32_c blockNumber = address / mBlockSize;
	uint32_c index = blockNumber % mSetCount;
	uint32_c tag = blockNumber / mSetCount;
	unsigned int cycles = 0;

	// This algorithm searches through the index for the desired tag.
	auto tagIter = std::find_if(mSets[index].begin(), mSets[index].end(), [&](auto i) { 
		return tag == ((i & ~(mValidBit | mDirtyBit)) / mBlockSize);
		});

	if (tagIter == mSets[index].end()) { // The tag was not found...
		mLoadMiss++;

		if (mSets[index].size() == mSetSize) { // ... and the set is full
			if (mPolicy == Policy::Random)
				std::shuffle(mSets[index].begin(), mSets[index].end(), gen);
			else // FIFO or LRU rotates everything 1 to the left so that the front element is now the back element
				std::rotate(mSets[index].begin(), mSets[index].begin() + 1, mSets[index].end());

			if (mSets[index].back() & mDirtyBit) { // ... and the back element has the dirty bit set
				mDirtyEvict++;
				unsigned int reconstructedAddress =
					((mSets[index].back() / mBlockSize) * mSetCount * mBlockSize) | (index * mBlockSize) | mSets[index].back() & (mBlockSize - 1);
				cycles += mLowerMem->writeAddress(reconstructedAddress);
			}
			mSets[index].back() = mValidBit | (tag * mBlockSize) | (address & (mBlockSize - 1));
		}
		else
			mSets[index].push_back(mValidBit | (tag * mBlockSize) | (address & (mBlockSize - 1)));

		cycles += mLowerMem->readAddress(address);			
	}
	else {
		mLoadHit++;
		if (mPolicy == Policy::LRU && mSets[index].back() != *tagIter)
			// Moves MRU block to the back of the set while maintaining the order of the other blocks.
			std::rotate(tagIter, std::next(tagIter), mSets[index].end());
	}
	return mAccessTime + cycles;
}

uint32_c Cache::writeAddress(uint32_c address) {
	int blockNumber = address / mBlockSize;
	int index = blockNumber % mSetCount; 
	int tag = blockNumber / mSetCount;
	int cycles = 0;

	auto tagIter = std::find_if(mSets[index].begin(), mSets[index].end(), [&](auto i) {
		return tag == ( (i & ~(mValidBit | mDirtyBit)) / mBlockSize);
		});

	if (tagIter == mSets[index].end()) {
		mStoreMiss++;

		if (mSets[index].size() == mSetSize) {
			if (mPolicy == Policy::Random)
				std::rotate(mSets[index].begin(), mSets[index].begin() + dis(gen), mSets[index].end());
			else
				std::rotate(mSets[index].begin(), mSets[index].begin() + 1, mSets[index].end());

			if (mSets[index].back() & mDirtyBit) {
				mDirtyEvict++;
				unsigned int reconstructedAddress =
					((mSets[index].back() / mBlockSize) * mSetCount * mBlockSize) | (index * mBlockSize) | mSets[index].back() & (mBlockSize - 1);
				cycles += mLowerMem->writeAddress(reconstructedAddress);
			}

			mSets[index].back() = mValidBit | mDirtyBit | (tag * mBlockSize) | (address & (mBlockSize - 1));
		}
		else
			mSets[index].push_back(mValidBit | mDirtyBit | (tag * mBlockSize) | (address & (mBlockSize - 1)));

		cycles += mLowerMem->readAddress(address);
	}
	else {
		mStoreHit++;
		*tagIter = mValidBit | mDirtyBit | (tag * mBlockSize) | (address & (mBlockSize - 1));
		if (mPolicy == Policy::LRU && mSets[index].back() != *tagIter)
			std::rotate(tagIter, std::next(tagIter), mSets[index].end());
	}
	return mAccessTime + cycles;
}

std::ostream& operator<<(std::ostream& os, const Cache& c) {
	return os << "Load Hit count: " << c.mLoadHit << "\tLoad Miss count: " << c.mLoadMiss << std::endl <<
		"Store Hit count: " << c.mStoreHit << "\tStore Miss count: " << c.mStoreMiss << std::endl <<
		"Dirty evictions: " << c.mDirtyEvict << "\tHit ratio: " <<
		((double)c.mLoadHit + c.mStoreHit) / ((double)c.mLoadHit + c.mLoadMiss + c.mStoreHit + c.mStoreMiss) << std::endl <<
		"Average Memory Access Time: " << c.getAMAT();
}