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
#include "BinaryConverter.h"
#include <vector>
#include <map>
#include <cmath>
#include <iostream>

class PageTable {
private:
//int pageSize = 0;

public:
int pageSize = 0;                    // number of bytes in frames and pages
std::vector<PageTableEntry> entries; //[TODO]: map <VPN,PTE>
int accessOrdinal = 1;               // largest == most recently used. 

/// <summary>
/// Parameterized constructor
/// </summary>
/// <param name="totalVirtualPages">number of PTEs</param>
/// <param name="PFNsize">number of valid frames</param>
/// <param name="frameSize">size of offset (in bytes)</param>
PageTable::PageTable(int totalVirtualPages, int totalFrames, int frameSize);

/// <summary>
/// Given VPN, return mapped PFN.
/// </summary>
/// <param name="VPN">Virtual Page Number bit array</param>
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
    // Convert to binary integer
    int VPNindex = BinaryConverter::ToBinaryInt(targetVPN);

    // Get entry via indexed VPN
    PageTableEntry* PTEptr = &entries[VPNindex];

    // Is it invalid?
    if(PTEptr->validBit == false) { // if so, page fault
        std::cout << "Page fault for entry VPN == " << VPNindex << std::endl;
    }
    
    // Update reference ordinal
    PTEptr->lastUsed = accessOrdinal;
    accessOrdinal++;

    return PTEptr->PFN; //[TODO]: check if this leaks memory
}
