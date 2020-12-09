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
#include "LookupBuffer/DTLB.h"
namespace PageFaultHandler {

    int HandleFault(PageTable* PT, TLB* DTLB, Cache* DC, int VPNindex);
    int FindFreeFrame(PageTable* PT);
    int LRUReplacePage(PageTable* PT, TLB* DTLB, Cache* DC);

    int HandleFault(PageTable* PT, TLB* DTLB, Cache* DC, int VPNindex) {
        int PFN = FindFreeFrame(PT);        // Find free frame to use.
        if(PFN == -1) {                     // Is there a free frame?
            PFN = LRUReplacePage(PT, DTLB, DC); // If not, swap out a frame.
        }
        PT->SetEntryPFN(VPNindex, PFN);     // Update entry's PFN.
        PT->SetEntryValidity(VPNindex,true);// Set as now valid.
        return PFN;                         // Return PFN.
    }

    /// <summary>
    /// Returns PFN of free frame (If any).
    /// </summary>
    /// <param name="PT">pointer to page table</param>
    /// <returns>PFN of free frame. -1 if no free frames.</returns>
    int FindFreeFrame(PageTable* PT) {
        return PT->AllocateFrame();
    }

    /// <summary>
    /// Runs LRU replacement for pages.
    /// </summary>
    /// <param name="PT">pointer to page table</param>
    /// <returns>PFN of victim entry.</returns>
    int LRUReplacePage(PageTable* PT, TLB* DTLB, Cache* DC) {
        int LRU = PT->GetAccessOrdinal();
        int victimPFN = -1;
        int victimVPN = 0;
        // Find Least-Recently-Used entry in PT.
        for(int i = 0; i < PT->GetTableSize(); i++) {
            // If invalid, skip. (It doesn't have a frame).
            if(PT->GetEntryValidity(i) == false) continue;
            // If least recent so far...
            if(PT->GetEntryAccessOrdinal(i) < LRU) {
                LRU = PT->GetEntryAccessOrdinal(i); // Update comparer,
                victimPFN = PT->GetEntryPFN(i);     // update PFN,
                victimVPN = i;                      // and update VPN
            }
        }        
        // Update victim entry
        PT->SetEntryValidity(victimVPN, false);
        if(DTLB != nullptr) {
            DTLB->SetEntryValidity(victimVPN, false);
        }
        ///TODO: Update DC
        return victimPFN;
    }


} // namespace PageFaultHandler

#endif