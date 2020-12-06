//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Project:		GroupProject
//	File Name:		PageTableEntry.h
//	Description:    Struct for simulating a page table entry
//	Course:			CSCI-4727-940: Operating Systems
//	Author:			Harrison Pollitte, pollitteh@etsu.edu, Department of Computing, East Tennessee State University
//	Created:		Wednesday, December 2 2020
//	Copyright:	    Harrison Pollitte 2020
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef PAGETABLEENTRY_H
#define PAGETABLEENTRY_H
#include <vector>


///<summary>
/// entry in a page table. 
/// used to translate VPN -> PFN
///</summary>
struct PageTableEntry
{
public:
    // Fields
    //std::vector<int> PFN; 
    int PFN = 0;            // Frame number of this entry.
    bool dirtyBit = false;  // Have the contents of the frame pointed to by this been altered?
    bool validBit = false;  // Is the translation of this entry valid?
    int lastUsed = 0;       // Larger means more recently used. 0 means never used.

    /** Constructor
    PageTableEntry(int numberOfPFNBits)
    {
        PFN.reserve(numberOfPFNBits);
    }
    /**/
};

#endif