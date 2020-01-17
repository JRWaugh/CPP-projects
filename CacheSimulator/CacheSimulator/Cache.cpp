#include "Cache.h"

Cache::Cache(uint32c_t blockSize = 0, uint32c_t setSize = 0, uint32c_t totalSize = 0, Policy policy = Policy::LRU, uint32c_t accessTime = 0, uint32c_t accessTimeLower = 0) :
    MainMemory{ accessTime },
    mBlockSize{ 1ULL << blockSize }, mSetSize{ 1ULL << setSize }, mTotalSize{ 1ULL << totalSize }, mSets{ 1ULL << totalSize - blockSize - setSize }, mPolicy{ policy },
    mStoreHit{ 0 }, mStoreMiss{ 0 }, mLoadHit{ 0 }, mLoadMiss{ 0 }, mDirtyEvict{ 0 },
    mLowerMem{ make_shared<MainMemory>(accessTimeLower) }, gen{ random_device{}() } {};

std::optional<uint32c_t> Cache::accessAddress(uint32c_t address, const unsigned char instruction) {
    /* If the instruction is not 'l' or 's' this function will return a nullopt (non-value).
     * This is because any unsigned integer is technically a valid result, so a non-value is needed.
     */
    uint32c_t blockNumber = address / mBlockSize;
    uint32c_t index = blockNumber % mSets.capacity();
    uint32c_t tag = blockNumber / mSets.capacity();
    unsigned int cycles = mAccessTime;

    /* This algorithm searches through the index for the desired tag. */
    auto tagIter = find_if(mSets[index].begin(), mSets[index].end(), [this, &tag](auto block) {
        return (block.first & mValidBit) ? tag == block.second / mBlockSize : false;
        });

    if (tagIter == mSets[index].end()) { // The tag was not found...
        if (mSets[index].size() == mSetSize) { // ... and the set is full
            if (mPolicy == Policy::Random)
                shuffle(mSets[index].begin(), mSets[index].end(), gen);
            else
                rotate(mSets[index].begin(), next(mSets[index].begin()), mSets[index].end());

            if (mSets[index].back().first & mDirtyBit) { // ... and the back element has the dirty bit set
                ++mDirtyEvict;
                /* The sim can have differently sized blocks (eg. L2 smaller than L1) so reconstructing
                 * the address this way ensures everything behaves "properly".
                 */
                auto reconstructedAddress = ((mSets[index].back().second / mBlockSize) * mSets.capacity() * mBlockSize | index * mBlockSize | mSets[index].back().second & mBlockSize - 1);
                cycles += mLowerMem->accessAddress(reconstructedAddress, store).value();
            }

            if (instruction == load) {
                ++mLoadMiss;
                mSets[index].back() = { mValidBit, tag * mBlockSize | address & mBlockSize - 1 };
            }
            else if (instruction == store) {
                ++mStoreMiss;
                mSets[index].back() = { mValidBit | mDirtyBit, tag * mBlockSize | address & mBlockSize - 1 };
            }
            else
                return nullopt;
        }
        else {
            if (instruction == load) {
                ++mLoadMiss;
                mSets[index].emplace_back( mValidBit,  tag * mBlockSize | address & mBlockSize - 1 );
            }
            else if (instruction == store) {
                ++mStoreMiss;
                mSets[index].emplace_back( mValidBit | mDirtyBit,  tag * mBlockSize | address & mBlockSize - 1 );
            }
            else
                return nullopt;
        }

        cycles += mLowerMem->accessAddress(address, load).value();
    }
    else {
        if (instruction == load)
            ++mLoadHit;
        else if (instruction == store) {
            ++mStoreHit;
            *tagIter = { mValidBit | mDirtyBit,  tag * mBlockSize | address & mBlockSize - 1 };
        }
        else
            return nullopt;

        if (mPolicy == Policy::LRU) {
            /* Moves accessed block to the back of the set (MRU) while maintaining the order of the other blocks. */
            rotate(tagIter, next(tagIter), mSets[index].end());
        }
    }
    return cycles;
}

void Cache::invalidateCache() {
    /* Unsets the valid bits in each block in each set of the cache. Should be called when a new file is read. */
    for (auto& set : mSets)
        for (auto& block : set)
            block.first &= ~mValidBit;
}

const double Cache::getAMAT() const {
    return (double)mAccessTime + (((double)mLoadMiss + mStoreMiss) / ((double)mLoadMiss + mLoadHit + mStoreMiss + mStoreHit)) * mLowerMem->getAMAT();
}

ostream& operator<<(ostream& os, const Cache& c) {
    return os <<
        left << setw(20) << "Load hit count:" << right << setw(10) << c.mLoadHit << "\t" <<
        left << setw(20) << "Load miss count:" << right << setw(10) << c.mLoadMiss << endl <<
        left << setw(20) << "Store hit count:" << right << setw(10) << c.mStoreHit << "\t" <<
        left << setw(20) << "Store miss count:" << right << setw(10) << c.mStoreMiss << endl <<
        left << setw(20) << "Dirty evictions:" << right << setw(10) << c.mDirtyEvict << "\t" <<
        left << setw(20) << "Hit ratio:" << right << setw(10) <<
        ((double)c.mLoadHit + c.mStoreHit) / ((double)c.mLoadHit + c.mLoadMiss + c.mStoreHit + c.mStoreMiss) << endl <<
        left << setw(20) << "Average Access Time:" << right << setw(10) << c.getAMAT();
}