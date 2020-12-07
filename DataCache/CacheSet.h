#pragma once

#include <CacheEntry.h>
#include <deque>

using namespace std;

class CacheSet
{
public:

    CacheSet() {}

    // constructor
    CacheSet(int nentries, int setnumber)
    {
        TotalEntries = nentries; // total entries
        SetNumber = setnumber; // current set number

        // create new entries
        for (int i = 0; i < nentries; i++)
        {
            Entries->push_back(new CacheEntry());
        }
        
    }

    //deconstructor
    ~CacheSet()
    {
        // cleanup after ourself
        for (int i = 0; i < TotalEntries; i++)
        {
            delete Entries->at(i);
        }

        // clean settable
        delete Entries;
        
    }

    bool GetEntry(int tag)
    {
        CacheEntry *currentEntry;
        
        for (int i = 0; i < TotalEntries; i++)
        {
            currentEntry = Entries->at(i);
            if ( (currentEntry->GetValidBit() == 1) && (currentEntry->GetTag() == tag) )
            {
                Entries->erase(Entries->begin() + 1);
                Entries->push_back(currentEntry);
                return true; // hit?
            }
        }
        return false; // miss?
    }


    // add a entry into set
    void AddEntry(int tag, int dirtybit)
    {
        CacheEntry *recentlyUsed = Entries->front(); // get front entry
        Entries->pop_front(); // remove it (remove front item)
        recentlyUsed->SetTag(tag);
        recentlyUsed->SetValidBit(1);
        recentlyUsed->SetDirtyBit(dirtybit);
        // push entry to back
        Entries->push_back(recentlyUsed);

    }

    // set a entry's bit to dirty
    void UpdateDirtyEntry(int tag)
    {
        CacheEntry *currentEntry;
        for (int i = 0; i < TotalEntries; i++)
        {
            currentEntry = Entries->at(i); // get current entry
            if (currentEntry->GetTag() == tag)
            {
                currentEntry->SetDirtyBit(1); // if tag matches then set bit dirty
            }
        }
        
    }

    // TODO:
    int Invalidate()
    {

    }


    // is entry set dirty bit?
    bool LRU_IsEntryDirtyBit()
    {
        return Entries->front()->GetDirtyBit();
    }

private:
    int TotalEntries;
    int SetNumber;
    std::deque<CacheEntry*> *Entries;
};