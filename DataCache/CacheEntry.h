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

    // ctor
    CacheEntry()
    {
        Tag = -1;
        ValidBit = 0;
        DirtyBit = 0;
        PFN = -1;
    }

    void SetTag(int t)
    {
        Tag = t;
    }

    int GetTag()
    {
        return Tag;
    }

    void SetValidBit(int bit)
    {
        ValidBit = bit;
    }

    int GetValidBit()
    {
        return ValidBit;
    }

    void SetDirtyBit(int bit)
    {
        DirtyBit = bit;
    }

    int GetDirtyBit()
    {
        return DirtyBit;
    }

    void SetPFN(int physical)
    {
        PFN = physical;
    }

    int GetPFN()
    {
        return PFN;
    }
};

#endif