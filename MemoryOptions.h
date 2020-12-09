/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Project:		GroupProject
//	File Name:		MemoryOptions.h
//	Description:    Configuration for the MemoryController class
//	Course:			CSCI-4727-940: Operating Systems
//	Author:			Harrison Pollitte, pollitteh@etsu.edu, Department of Computing, East Tennessee State University
//	Created:		Saturday, December 5 2020
//	Copyright:	    Harrison Pollitte 2020
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
    int pageCount   = 64;   // # of virtual pages
    int frameCount  = 4;    // # of physical frames  
    int pageSize    = 256;  // size of offsets
    
    int vpnBits     = 6;    // # bits for vpn
    int pfnBits     = 2;    // # bits for pfn
    int offBits     = 8;    // # bits for page offset

    // DC Options
    int dcEntries   = 4;    // # of cache entries
    int dcTotalSets = 0;    // size of cache sets
    int dcLineSize  = 16;   // size of cache lines
    int dcPolicy    = 1;    // 0: write-through no write allocate, 1: write-back, write-allocate
    int dcSetSize   = 1;    // set size from config file

    int cacheTagBits    =   0; // number of tag index bits
    int cacheIndexBits  =   0; // number of cache index bits
    int cacheOffsetBits =   5; // number of offset bits
    int cacheEntriesPerSet = 0;

    // Other
    bool useVirt    =true;  // true: use virtual addressing
    bool useTLB     =true;  // true: use TLB (requires useVirt == true)

    // Default constructor
    MemoryOptions() {

    }

};
#endif //MEM_OPTIONS_H