
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
PageTable();
PageTable(int totalVirtualPages, int totalFrames, int pageSize);
std::pair<bool,int> LookUp(int VPN);
int GetAccessOrdinal();
void SetEntryValidity(int VPN, bool state);
bool GetEntryValidity(int VPN);
void SetEntryDirty(int VPN, bool state);
bool GetEntryDirty(int VPN);
void SetEntryPFN(int VPN, int PFN);
int GetEntryPFN(int VPN);
HardwareStats GetStatistics();
int GetEntryAccessOrdinal(int VPN);
TableEntry GetEntry(int VPN);
int GetTableSize();
int AllocateFrame();
};
#endif