#ifndef CACHEENTRY_H
#define CACHEENTRY_H

class CacheEntry
{
private:
    int Tag;
    int ValidBit;
    int DirtyBit;
    int PFN;

public:

    // constructor / set default values
    CacheEntry()
    {
        Tag = -1;
        ValidBit = 0;
        DirtyBit = 0;
        PFN = -1;
    }

    // Sets the tag for this cache entry
    void SetTag(int t)
    {
        Tag = t;
    }

    // Returns the tag for this cache entry
    int GetTag()
    {
        return Tag;
    }

    // Sets the valid bit in this cache entry
    void SetValidBit(int bit)
    {
        ValidBit = bit;
    }

    // Returns if this cache entry valid bit is set
    int GetValidBit()
    {
        return ValidBit;
    }

    // Sets the dirty bit to 0 or 1 in this cache entry
    void SetDirtyBit(int bit)
    {
        DirtyBit = bit;
    }

    // Return if this cache entry has its dirty bit set
    int GetDirtyBit()
    {
        return DirtyBit;
    }

    // set Physican Page Number for this cache entry
    void SetPFN(int physical)
    {
        PFN = physical;
    }

    // get Physical Page Number for this cache entry
    int GetPFN()
    {
        return PFN;
    }
};

#endif