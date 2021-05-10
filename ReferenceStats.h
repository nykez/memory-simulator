

#ifndef REF_STATS_H
#define REF_STATS_H
/// <summary>
/// Reference info for disk, memory, and page table.
/// </summary>
struct ReferenceStats {
    int pageTableRefCount = 0;  // times we touched page table.
    int mainMemoryRefCount = 0; // times we touched main memory.
    int diskRefCount = 0;       // times we touched disk.

    /// <summary>
    /// Default Constructor
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    ReferenceStats() {

    }

    /// <summary>
    /// Parameterized constructor.
    /// </summary>
    /// <param name="page">page reference count</param>
    /// <param name="mem">memory reference count</param>
    /// <param name="disk">disk reference count</param>
    ReferenceStats(int page, int mem, int disk) {
        pageTableRefCount = page;
        mainMemoryRefCount = mem;
        diskRefCount = disk;
    }
};
#endif