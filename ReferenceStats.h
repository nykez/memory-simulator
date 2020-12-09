//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Project:		GroupProject
//	File Name:		ReferenceStats.h
//	Description:    Holds reference information for disk, memory, and page table.
//	Course:			CSCI-4727-940: Operating Systems
//	Author:			Harrison Pollitte, pollitteh@etsu.edu, Department of Computing, East Tennessee State University
//	Created:		Sunday, December 6 2020
//	Copyright:	    Harrison Pollitte 2020
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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