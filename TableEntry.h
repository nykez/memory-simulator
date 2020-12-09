/// NAME: TableEntry.h
/// PURPOSE: Entry for Data TLB and Page Table
/// AUTHOR: Harrison Lee Pollitte
/// CREATED: 12/8/2020
/// NOTES:
/// * Made to replace DTLBEntry.h and PageTableEntry.h


#ifndef TABLE_ENTRY_H
#define TABLE_ENTRY_H

/// PURPOSE: Contain data for table entries. I.e. TLB and Page Table
struct TableEntry {
public:
    int PFN = 0;            // Frame number of this entry.
    bool dirtyBit = false;  // Have the contents of the frame pointed to by this been altered?
    bool validBit = false;  // Is the translation of this entry valid?
    int lastUsed = 0;       // Larger means more recently used. 0 means never used.

    /// PURPOSE: Default constructor
    TableEntry() = default;

    /// PURPOSE: Parameterized constructor
    TableEntry(int pfn) {
        PFN = pfn;
    }

    TableEntry(int pfn, bool dirty, bool valid, int used) {
        PFN = pfn;
        dirtyBit = dirty;
        validBit = valid;
        lastUsed = used;
    }
};

#endif //TABLE_ENTRY_H