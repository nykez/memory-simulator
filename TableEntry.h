

#ifndef TABLE_ENTRY_H
#define TABLE_ENTRY_H

/// <summary>
/// Contain data for table entries. I.e. TLB and Page Table
/// </summary>
struct TableEntry {
public:
    int PFN = 0;            // Frame number of this entry.
    bool dirtyBit = false;  // Have the contents of the frame pointed to by this been altered?
    bool validBit = false;  // Is the translation of this entry valid?
    int lastUsed = 0;       // Larger means more recently used. 0 means never used.

    /// <summary>
    /// Default constructor.
    /// </summary>
    TableEntry() = default;

    /// <summary>
    /// Parameterized constructor.
    /// </summary>
    /// <param name="pfn">value of PFN</param>
    TableEntry(int pfn) {
        PFN = pfn;
    }

    /// <summary>
    /// Parameterized constructor.
    /// </summary>
    /// <param name="pfn">value of PFN</param>
    /// <param name="dirty">value of dirty bit</param>
    /// <param name="valid">value of valid bit</param>
    /// <param name="used">value of used.</param>
    TableEntry(int pfn, bool dirty, bool valid, int used) {
        PFN = pfn;
        dirtyBit = dirty;
        validBit = valid;
        lastUsed = used;
    }
};

#endif //TABLE_ENTRY_H