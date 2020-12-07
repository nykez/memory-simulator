//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Project:		GroupProject
//	File Name:		PageFaultHandler.h
//	Description:    Class for simulating a page table
//	Course:			CSCI-4727-940: Operating Systems
//	Author:			Harrison Pollitte, pollitteh@etsu.edu, Department of Computing, East Tennessee State University
//	Created:		Friday, December 4 2020
//	Copyright:	    Harrison Pollitte 2020
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PAGEFAULTHANDLER_H
#define PAGEFAULTHANDLER_H

#include <vector>
#include <iostream>
#include "PageTable/PageTable.h"
#include "DataCache/Cache.h"
namespace PageFaultHandler {

    static int diskReferences = 0;

    int HandleFault(PageTable::PageTable* PT, Cache::Cache* DC, int VPNindex) {
        int PFN = FindFreeFrame(PT);        // Find free frame to use.
        if(PFN == -1) {                     // Is there a free frame?
            PFN = LRUReplacePage(PT, DC);   // If not, swap out a frame.
            diskReferences++;               // We touched disk.
        }
        PT->entries.at(VPNindex).PFN = PFN; // Update entry's PFN.
        return PFN;                         // Return PFN.
    }

    /// <summary>
    /// Returns PFN of free frame (If any).
    /// </summary>
    /// <param name="PT">pointer to page table</param>
    /// <returns>PFN of free frame. -1 if no free frames.</returns>
    int FindFreeFrame(PageTable* PT) {
       if(PT->GetFrameCount() < PT->GetMaxFrameCount())
           return PT->GetFrameCount();
       return -1;
    }

    /// <summary>
    /// Runs LRU replacement for pages.
    /// </summary>
    /// <param name="PT">pointer to page table</param>
    /// <returns>PFN of victim entry.</returns>
    int LRUReplacePage(PageTable* PT, Cache::Cache* DC) {
        int LRU = PT->GetAccessOrdinal();
        int victimPFN;
        int victimVPN = 0;
        // Find Least-Recently-Used entry in PT.
        for(int i = 0; i < PT->entries.size(); i++) {
            // If invalid, skip. (It doesn't have a frame).
            if(PT->GetEntryValidity(i) == false) continue;
            // If least recent so far...
            if(PT->entries.at(i).lastUsed < LRU) {
                LRU = PT->entries.at(i).lastUsed;   // update comparer
                victimPFN = PT->GetEntryPFN(i);     // and update PFN
                victimVPN = i;
            }
        }        
        // Update victim entry
        PT->SetEntryValidity(victimVPN, false);
        // Inform TLB and DC of newly invalidated entry
        //InformTLB(victimVPN);
        DC->UpdateDirtyEntry(0,0);

        return victimPFN;
    }


} // namespace PageFaultHandler

#endif