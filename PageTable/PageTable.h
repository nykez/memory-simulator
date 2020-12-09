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
#include "../TableEntry.h"
#include "../HardwareStats.h"
#include <vector>
#include <iostream>

/// <summary>
/// Holds page entries. Resides in main memory.
/// </summary>
class PageTable {
private:
int frameCount = 0;                  // number of frames in use.
int maxFrameCount = 0;               // max frames allowed.
int accessOrdinal = 1;               // largest == most recently used. 
int pageTableReferences = 0;         // amount of times we've referenced this table
int hits = 0;                        // number of page table hits
int misses = 0;                      // number of page table misses
std::vector<TableEntry> entries;     // table entries

public:    
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
/// <param name="VPN">Virtual Page Number</param>
/// <returns>bool: hit or miss. int: PFN</returns>
std::pair<bool,int> LookUp(int VPN);

/// <summary>
/// Get the highest value of access ordinal.
/// </summary>
/// <returns>access value of most recently touched entry.</returns>
int GetAccessOrdinal();

/// <summary>
/// Set valid-bit for an entry.
/// </summary>
/// <param name="VPN">Virtual Page Number of target entry.</param>
/// <param name="state">state to set validbit to.</param>
void SetEntryValidity(int VPN, bool state);

/// <summary>
/// Get valid-bit for an entry.
/// </summary>
/// <param name="VPN">Virtual Page Number of target entry.</param>
/// <returns>true: entry is valid; false: entry invalid</returns>
bool GetEntryValidity(int VPN);

/// <summary>
/// Set dirtybit of target entry.
/// </summary>
/// <param name="VPN">Virtual Page Number of target entry.</param>
/// <param name="state">state to set dirtybit to.</param>
void SetEntryDirty(int VPN, bool state);

/// <summary>
/// Get dirtybit of target entry.
/// </summary>
/// <param name="VPN">Virtual Page Number of target entry.</param>
/// <returns>true: is dirty. false: is clean.</returns>
bool GetEntryDirty(int VPN);

/// <summary>
/// Set PFN for target entry.
/// </summary>
/// <param name="VPN">Virtual Page Number of target entry.</param>
/// <param name="PFN">new PFN</param>
void SetEntryPFN(int VPN, int PFN);

/// <summary>
/// Get PFN of target entry.
/// </summary>
/// <param name="VPN">Virtual Page Number of target entry.</param>
/// <returns>PFN of target entry.</returns>
int GetEntryPFN(int VPN);

/// <summary>
/// Get statistics of page table.
/// </summary>
/// <returns>statistics.</returns>
HardwareStats GetStatistics();

/// <summary>
/// Retrieve access ordinal of entry.
/// </summary>
/// <param name="VPN">Virtual Page Number of target entry.</param>
int GetEntryAccessOrdinal(int VPN);

/// <summary>
/// Returns the entry with the given VPN.
/// NOTE: Does not check if entry exists.
/// </summary>
/// <param name="VPN">Virtual Page Number.</param>
TableEntry GetEntry(int VPN);

/// <summary>
/// Gets number of entries
/// </summary>
/// <returns> number of entries. </returns>
int GetTableSize();

/// <summary>"Allocates frame". Increments 
/// number of in-use frames in the page table.
/// </summary>
/// <returns> -1: full. Else, PFN of allocated frame. </returns>
int AllocateFrame();
};




#endif