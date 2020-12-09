//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Project:		GroupProject
//	File Name:		Cache.h
//	Description:    Cache class
//	Course:			CSCI-4727-940: Operating Systems
//	Author:			Jordan Cavins
//	Created:		Wednesday, December 2 2020
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include "CacheSet.h"
#include "../HardwareStats.h"

class Cache
{
public:
    int hits = 0;
    int misses = 0;

    // Create a new cache
    // We only have 1 in our program but this can easily be expanded out to use data cache and a instruction cache
    // so why not?
    Cache(int totalSets, int setSize)
    {
        TotalSets = totalSets;
        SetSize = setSize;

        Sets = new vector<CacheSet*>; // initalize vector cache set

        // let's create our sets
        for (int i = 0; i < TotalSets; ++i) 
        {
            Sets->push_back( new CacheSet(SetSize, i)); 
        }
    }

    // deconstructor
    // clear/free up memory
    ~Cache()
    {
        for (int i = 0; i < TotalSets; ++i)
        {
            delete Sets->at(i); // delete set
        }
        
        delete Sets; // delete our vector
    }

    // Get a entry from a set
    bool GetEntry(int index, int tag)
    {
        return Sets->at(index)->GetEntry(tag);
    }

    // Add a entry to a set
    void AddEntry(int index, int tag, int dirtyBit, int pfn)
    {
        Sets->at(index)->AddEntry(tag, dirtyBit, pfn);
    }

    // Update big inside a set of a entry
    void UpdateDirtyEntry(int index, int tag)
    {
        Sets->at(index)->UpdateDirtyEntry(tag);
    }

    int Invalidate(int physicalFrameNmumber)
    {
        int TotalDirty = 0;

        for (int i = 0; i < TotalSets; ++i)
        {
            TotalDirty += Sets->at(i)->Invalidate(physicalFrameNmumber);
        }
        

        return TotalDirty;
    }

    // GET POLICY
    //  0: write-through no write allocate, 1: write-back, write-allocate
    int GetPolicy()
    {
        return Policy;
    }

    // SET POLICY
    //  0: write-through no write allocate, 1: write-back, write-allocate
    void SetPolicy(int policy)
    {
        Policy = policy;
    }


    bool LRU_IsEntryDirtyBit(int index)
    {
        return Sets->at(index)->LRU_IsEntryDirtyBit();
    }

    // return hardware stats
    HardwareStats GetStatistics()
    {
        HardwareStats stats(hits, misses);
        return stats;
    }

private:
    vector<CacheSet*> *Sets; // vector of our CacheSets objects (class)
    int TotalSets; // total ses
    int SetSize; // set size
    int Policy = 0; // 0: write-through no write allocate, 1: write-back, write-allocate
};

#endif