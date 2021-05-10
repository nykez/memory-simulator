
#include "PageTable.h"
// Default constructor. Doesn't do anything.
PageTable::PageTable() {

}

/// <summary>
/// Parameterized constructor
/// </summary>
/// <param name="totalVirtualPages">number of PTEs</param>
/// <param name="totalFrames">number of valid frames</param>
/// <param name="pageSize">size of offset (in bytes)</param>
PageTable::PageTable(int totalVirtualPages, int totalFrames, int pageSize) {
    maxFrameCount = totalFrames;
    this->entries.reserve(totalVirtualPages);
    // Populate map with empty entries
    for(int i = 0; i < totalVirtualPages; i++) {
        TableEntry PTE;
        entries.push_back(PTE);
    }
}

/// <summary>
/// Given VPN, return mapped PFN.
/// </summary>
/// <param name="VPN">Virtual Page Number</param>
/// <returns>bool: hit or miss. int: PFN</returns>
std::pair<bool, int> PageTable::LookUp(int VPN)
{
    pageTableReferences++;

    // Result return value
    std::pair<bool, int> res (true, 0);

    // Get entry via indexed VPN
    TableEntry PTE = entries.at(VPN);

    // Is it invalid?
    if(PTE.validBit == false) { // if so, page fault
        res.first = false;
        misses++;
    } else {
        res.second = PTE.PFN;
        hits++;
    }
    
    // Update reference ordinal
    entries.at(VPN).lastUsed = accessOrdinal;
    accessOrdinal++;
    
    return res;
}

/// <summary>
/// Gets number of entries
/// </summary>
/// <returns> number of entries. </returns>
int PageTable::GetTableSize() {
    return this->entries.size();
}

/// <summary>"Allocates frame". Increments 
/// number of in-use frames in the page table.
/// </summary>
/// <returns> -1: full. Else, PFN of allocated frame. </returns>
int PageTable::AllocateFrame() {
    int PFN = -1;
    if(frameCount < maxFrameCount) {
        PFN = frameCount;
        frameCount++;
    }
    return PFN;
}

/// <summary>
/// Get the highest value of access ordinal.
/// </summary>
/// <returns>access value of most recently touched entry.</returns>
int PageTable::GetAccessOrdinal() {
    return accessOrdinal;
}

/// <summary>
/// Set valid-bit for an entry.
/// </summary>
/// <param name="VPN">Virtual Page Number of target entry.</param>
/// <param name="state">state to set validbit to.</param>
void PageTable::SetEntryValidity(int VPN, bool state) {
    entries.at(VPN).validBit = state;
}

/// <summary>
/// Get valid-bit for an entry.
/// </summary>
/// <param name="VPN">Virtual Page Number of target entry.</param>
/// <returns>true: entry is valid; false: entry invalid</returns>
bool PageTable::GetEntryValidity(int VPN) {
    return entries.at(VPN).validBit;
}

/// <summary>
/// Set dirtybit of target entry.
/// </summary>
/// <param name="VPN">Virtual Page Number of target entry.</param>
/// <param name="state">state to set dirtybit to.</param>
void PageTable::SetEntryDirty(int VPN, bool state) {
    entries.at(VPN).dirtyBit = state;
}

/// <summary>
/// Get dirtybit of target entry.
/// </summary>
/// <param name="VPN">Virtual Page Number of target entry.</param>
/// <returns>true: is dirty. false: is clean.</returns>
bool PageTable::GetEntryDirty(int VPN) {
    return entries.at(VPN).dirtyBit;
}

/// <summary>
/// Set PFN for target entry.
/// </summary>
/// <param name="VPN">Virtual Page Number of target entry.</param>
/// <param name="PFN">new PFN</param>
void PageTable::SetEntryPFN(int VPN, int PFN) {
    entries.at(VPN).PFN = PFN;
}

/// <summary>
/// Get PFN of target entry.
/// </summary>
/// <param name="VPN">Virtual Page Number of target entry.</param>
/// <returns>PFN of target entry.</returns>
int PageTable::GetEntryPFN(int VPN) {
    return entries.at(VPN).PFN;
}
/// <summary>
/// Retrieve access ordinal of entry.
/// </summary>
/// <param name="VPN">Virtual Page Number of target entry.</param>
int PageTable::GetEntryAccessOrdinal(int VPN) {
    return entries.at(VPN).lastUsed;
}

/// <summary>
/// Get statistics of page table.
/// </summary>
/// <returns>statistics.</returns>
HardwareStats PageTable::GetStatistics() {
    HardwareStats stats(hits,misses);
    return stats;
}

/// <summary>
/// Returns the entry with the given VPN.
/// NOTE: Does not check if entry exists.
/// </summary>
/// <param name="VPN">Virtual Page Number.</param>
TableEntry PageTable::GetEntry(int VPN) {
    TableEntry pte = entries.at(VPN);
    return pte;
}