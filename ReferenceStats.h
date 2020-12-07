/// NAME: ReferenceStats.h
/// PURPOSE: For holding hit information for disk, memory, and page table
/// AUTHOR: Harrison Lee Pollitte
/// CREATED: 12/6/2020
/// NOTES: Used by OutputHandler. Collected by MemoryController
/// *
/// *

struct ReferenceStats
{
    int pageTableRefCount = 0;  // times we touched page table.
    int mainMemoryRefCount = 0; // times we touched main memory.
    int diskRefCount = 0;       // times we touched disk.

    ReferenceStats() {

    }

    ReferenceStats(int page, int mem, int disk) {
        pageTableRefCount = page;
        mainMemoryRefCount = mem;
        diskRefCount = disk;
    }
};
