#pragma once

#include <vector>
#include <CacheSet.h>

class Cache
{
public:
    
    // Create a new cache
    // We only have 1 in our program but this can easily be expanded out to use data cache and a instruction cache
    // so why not?
    Cache(int totalSets, int setSize)
    {
        TotalSets = totalSets;
        SetSize = setSize;

        Sets = new vector<CacheSet*>; // initalize vector cache set

        // let's create our sets
        for (int i = 0; i < TotalSets; i++) 
        {
            Sets->push_back( new CacheSet(SetSize, i)); 
        }
    }

    // deconstructor
    // clear/free up memory
    ~Cache()
    {
        for (int i = 0; i < TotalSets; i++)
        {
            delete Sets->at(i); // delete set
        }
        
        delete Sets; // delete our vector
    }

    // Get a entry from a set
    bool GetEntry()
    {

        return false;
    }

    // Add a entry to a set
    void AddEntry()
    {

    }

    // Update big inside a set of a entry
    void UpdateDirtyEntry(int index, int tag)
    {
        Sets->at(index)->UpdateDirtyEntry(tag);
    }


private:
    vector<CacheSet*> *Sets; // vector of our CacheSets objects (class)
    int TotalSets; // total ses
    int SetSize; // set size
};