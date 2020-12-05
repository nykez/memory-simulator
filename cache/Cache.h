

#ifndef CACHE_H
#define CACHE_H

enum CACHE_POLICY {
    WRITE_ALLOCATE, // 0
    WRITE_BACK, // 1
    WRITE_THROUGH // 2
};

class Cache 
{
    private:
        int cache_policy = WRITE_ALLOCATE;

        int total_entries = 0;
        int block_size = 0;
        int associativity = 8;

    public:
    Cache();
    ~Cache();

    // set the cache policy to use for this cache
    void setPolicy(int type);

};

#endif