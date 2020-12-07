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
#ifndef PAGETABLE_H
#define PAGETABLE_H
#include "PageTableEntry.h"
#include "../BinaryConverter.h"

#include <vector>
#include <cmath>
#include <iostream>

class PageTable {
private:
//int pageSize = 0;
int frameCount = 0;                  // number of frames in use.
int maxFrameCount = 0;               // max frames allowed.
int accessOrdinal = 1;               // largest == most recently used. 
int pageTableReferences = 0;         // amount of times we've referenced this table
int hits = 0;                        // number of page table hits
int misses = 0;                      // number of page table misses


public:
std::vector<PageTableEntry> entries; //[TODO]: map <VPN,PTE>
               
// Default constructor. Doesn't do anything.
PageTable();

/// <summary>
/// Parameterized constructor
/// </summary>
/// <param name="totalVirtualPages">number of PTEs</param>
/// <param name="totalFrames">number of valid frames</param>
/// <param name="pageSize">size of offset (in bytes)</param>
PageTable(int totalVirtualPages, int totalFrames, int pageSize);

/// <summary>
/// Given VPN, return mapped PFN.
/// </summary>
/// <param name="VPN">Virtual Page Number bit array</param>
std::pair<bool,int> TranslateVPN(int VPN);

int GetMaxFrameCount();
int GetFrameCount();
int GetAccessOrdinal();
void SetEntryValidity(int VPN, bool state);
bool GetEntryValidity(int VPN);
void SetEntryDirty(int VPN, bool state);
bool GetEntryDirty(int VPN);
void SetEntryPFN(int VPN, int PFN);
int GetEntryPFN(int VPN);
};


PageTable::PageTable() {

}

PageTable::PageTable(int totalVirtualPages, int totalFrames, int pageSize) {
    maxFrameCount = totalFrames;

    // Determine number of bits
    //int VPNBits    = (int)log2((double)totalVirtualPages);
    int PFNBits    = (int)log2((double)totalFrames);
    //int offsetBits = (int)log2((double)frameSize);

    // Populate map with empty entries
    for(int i = 0; i < totalVirtualPages; i++) {
        //entries.push_back(PageTableEntry(PFNBits));
        PageTableEntry PTE;
        entries.push_back(PTE);
    }
}

std::pair<bool, int> PageTable::TranslateVPN(int VPN)
{
    pageTableReferences++;

    // Result return value
    std::pair<bool, int> res (true, 0);
    
    // Convert to binary integer
   // int VPNindex = BinaryConverter::ToBinaryInt(VPN);

    // Get entry via indexed VPN
    PageTableEntry PTE = entries.at(VPN-1);

    // Is it invalid?
    if(PTE.validBit == false) { // if so, page fault
        std::cout << "Page fault for entry VPN == " << VPN << std::endl;
        res.first = false;
        misses++;
    } else {
        res.second = PTE.PFN;
        hits++;
    }
    
    // Update reference ordinal
    entries.at(VPN-1).lastUsed = accessOrdinal;
    accessOrdinal++;

    return res;
}

int PageTable::GetMaxFrameCount() {
    return maxFrameCount;
}

int PageTable::GetFrameCount() {
    return frameCount;
}

int PageTable::GetAccessOrdinal() {
    return accessOrdinal;
}

void PageTable::SetEntryValidity(int VPN, bool state) {
    entries.at(VPN).validBit = state;
}

bool PageTable::GetEntryValidity(int VPN) {
    return entries.at(VPN).validBit;
}

void PageTable::SetEntryDirty(int VPN, bool state) {
    entries.at(VPN).dirtyBit = state;
}

bool PageTable::GetEntryDirty(int VPN) {
    return entries.at(VPN).dirtyBit;
}

void PageTable::SetEntryPFN(int VPN, int PFN) {
    entries.at(VPN).PFN = PFN;
}

int PageTable::GetEntryPFN(int VPN) {
    return entries.at(VPN).PFN;
}

#endif