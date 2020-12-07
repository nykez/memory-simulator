#ifndef MEM_OPTIONS_H
#define MEM_OPTIONS_H

///<summary>
/// A struct for configuring MemoryController and its components
///</summary>
struct MemoryOptions
{
    // DTLB Options
    int tlbEntries  = 2;    // # of TLB entries

    // PT Options
    int pageCount   = 64;    // # of virtual pages
    int frameCount  = 4;    // # of physical frames
    int pageSize    = 256;  // size of offsets
    
    // DC Options
    int dcEntries   = 2;    // # of cache entries
    int dcSetSize   = 1;    // size of cache sets
    int dcLineSize  = 4;    // size of cache lines
    int dcPolicy    = 0;    // write policy for cache

    // Other
    bool useVirt    =true;  // true: use virtual addressing
    bool useTLB     =true;  // true: use TLB (requires useVirt == true)

    // Default constructor
    MemoryOptions() {

    }

};



#endif //MEM_OPTIONS_H