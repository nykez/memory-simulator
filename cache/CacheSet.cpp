#ifndef CACHESET_H
#define CACHESET_H

#include <vector>
#include <deque>
#include <CacheEntry.h>

using namespace std;

class CacheSet
{
public:

    CacheSet(int entries, int sets)
    {
        TotalEntries = entries;
        TotalSets = sets;
    }

private:
    int TotalEntries;
    int TotalSets;
    deque<CacheSet*> *Entries;
};

#endif