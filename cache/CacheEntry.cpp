

class CacheEntry
{

public:
    CacheEntry()
    {
        tag = __INT32_MAX__;
        validbit = 0;
        dirtybit = 0;
    }

    void SetTag(int t)
    {
        tag = t;
    };

    void SetValidBit(int bit)
    {
        validbit = bit;
    };

    void SetDirtyBit(int bit)
    {
        dirtybit = bit;
    };

    int GetTag()
    {
        return tag;
    }

    int GetValidBit()
    {
        return validbit;
    }

    int GetDirtyBit()
    {
        return dirtybit;
    }


private:
    int tag;
    int validbit;
    int dirtybit;

    
};
