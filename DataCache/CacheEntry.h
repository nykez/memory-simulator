#pragma once

class CacheEntry
{
private:
    int Tag;
    int ValidBit;
    int DirtyBit;

public:

    // ctor
    CacheEntry()
    {
        Tag = __INT32_MAX__;
        ValidBit = 0;
        DirtyBit = 0;
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
};