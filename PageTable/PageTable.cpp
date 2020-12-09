#include "PageTable.h"

PageTable::PageTable() {

}

PageTable::PageTable(int totalVirtualPages, int totalFrames, int pageSize) {
    maxFrameCount = totalFrames;
    this->entries.reserve(totalVirtualPages);
    // Populate map with empty entries
    for(int i = 0; i < totalVirtualPages; i++) {
        TableEntry PTE;
        entries.push_back(PTE);
    }
}

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

int PageTable::GetTableSize() {
    return this->entries.size();
}

int PageTable::AllocateFrame() {
    int PFN = -1;
    if(frameCount < maxFrameCount) {
        PFN = frameCount;
        frameCount++;
    }
    return PFN;
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

int PageTable::GetEntryAccessOrdinal(int VPN) {
    return entries.at(VPN).lastUsed;
}

HardwareStats PageTable::GetStatistics() {
    HardwareStats stats(hits,misses);
    return stats;
}

TableEntry PageTable::GetEntry(int VPN) {
    TableEntry pte = entries.at(VPN);
    return pte;
}