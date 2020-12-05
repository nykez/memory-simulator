

class CacheEntry
{
public:
    CacheEntry();

    void SetTag(int t);
    void SetValidBit(int bit);
    void SetDirtyBit(int bit);

    int GetTag();
    int GetValidBit();
    int GetDirtyBit();

private:
    int tag;
    int validBit;
    int dirtyBit;
};
