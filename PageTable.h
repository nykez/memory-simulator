//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Project:		GroupProject
//	File Name:		PageTable.h
//	Description:    Class for simulating a page table
//	Course:			CSCI-4727-940: Operating Systems
//	Author:			Harrison Pollitte, pollitteh@etsu.edu, Department of Computing, East Tennessee State University
//	Created:		Wednesday, December 2 2020
//	Copyright:	    Harrison Pollitte 2020
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "PageTableEntry.h"
#include "AddressStructs.h"
#include <vector>
#include <map>
#include <cmath>

class PageTable {
private:
//int pageSize = 0;

public:
int pageSize = 0;
std::vector<PageTableEntry> entries; //[TODO]: map <VPN,PTE>


/// <summary>
/// Parameterized constructor
/// </summary>
/// <param name="totalVirtualPages">number of PTEs</param>
/// <param name="PFNsize">number of valid frames</param>
/// <param name="frameSize">size of offset (in bytes)</param>
PageTable::PageTable(int totalVirtualPages, int totalFrames, int frameSize);


/// Given virtual address, return mapping of physical address
std::vector<int> PageTable::TranslateVPN(std::vector<int> VPN);

};


PageTable::PageTable(int totalVirtualPages, int totalFrames, int frameSize) {
    // Store size of each frame
    pageSize = frameSize;

    // Determine number of bits
    //int VPNBits    = (int)log2((double)totalVirtualPages);
    int PFNBits    = (int)log2((double)totalFrames);
    //int offsetBits = (int)log2((double)frameSize);

    // Populate map with empty entries
    for(int i = 0; i < totalVirtualPages; i++) {
        entries.push_back(PageTableEntry(PFNBits));
    }
}

std::vector<int> PageTable::TranslateVPN(std::vector<int> targetVPN)
{

    // Search table for PTE with addr's VPN
    for(int i = 0; i < PageTable::entries.size(); i++) 
    {
        if(entries[i].VPN == VPN)// if same bits
        {
            // We found our entry!
            // But, is it mapped?
            if(!entries[i].IsPresent())      // if not...
                HandlePageFault(&entries[i]);// update entry  
            //[TODO]: Update victim PTE
            return entries[i].PFN;           // return entry's PFN
        }
    }
}
