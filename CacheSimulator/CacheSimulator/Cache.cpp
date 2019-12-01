#include "Cache.h"

unsigned int Cache::readAddress(unsigned int address) {
	int index = (address / mBlockSize) & (mSetCount - 1);
	int tag = (address / mBlockSize) / mSetCount;
	auto tagIter = std::find(mIndex[index].begin(), mIndex[index].end(), tag);
	int cycles = 0;

	if (tagIter == mIndex[index].end()) {
		mMissCount++;
		if (mIndex[index].size() == mSetSize) {
			if (mIndex[index].back() & mDirtyBit)
				cycles += mLowerMem->writeAddress(address);
			mIndex[index].pop_back();
		}
		mIndex[index].push_front(tag); //possible to lose dirty bit here from lower than L1 cache?
		cycles += mLowerMem->readAddress(address);
		return mAccessTime + cycles;
	}
	else {
		mHitCount++;
		if (*mIndex[index].begin() != *tagIter)
			mIndex[index].splice(mIndex[index].begin(), mIndex[index], tagIter, std::next(tagIter));
		return mAccessTime;
	}
}

unsigned int Cache::writeAddress(unsigned int address) {
	int index = (address / mBlockSize) & (mSetCount - 1);
	int tag = (address / mBlockSize) / mSetCount;
	auto tagIter = std::find(mIndex[index].begin(), mIndex[index].end(), tag);
	int cycles = 0;

	if (tagIter == mIndex[index].end()) {
		if (mIndex[index].size() == mSetSize) {
			if (mIndex[index].back() & mDirtyBit) {
				mMissCount++; //only a miss if cache is full, tag isn't found and the dirty bit is set
				cycles += mLowerMem->writeAddress(address);
			}
			else
				mHitCount++;
			mIndex[index].pop_back();
		}
		else {
			mHitCount++;
		}
		mIndex[index].push_front(tag | mDirtyBit); //set the tag into the block list along with the dirty bit
		return mAccessTime + cycles;
	}
	else {
		mHitCount++; //tag was found in the set so we don't have to worry about writing back
		*tagIter |= mDirtyBit;
		if (mIndex[index].front() != *tagIter)
			mIndex[index].splice(mIndex[index].begin(), mIndex[index], tagIter, std::next(tagIter));
		return mAccessTime;
	}

	return 0;
}