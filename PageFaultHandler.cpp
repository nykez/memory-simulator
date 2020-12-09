/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Project:		GroupProject
//	File Name:		PageFaultHandler.cpp
//	Description:    Implemenation of PageFaultHandler.h
//	Course:			CSCI-4727-940: Operating Systems
//	Author:			Harrison Pollitte, pollitteh@etsu.edu, Department of Computing, East Tennessee State University
//	Created:		Saturday, December 5 2020
//	Copyright:	    Harrison Pollitte 2020
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "PageFaultHandler.h"


    /// <summary>
    /// Handle a page fault. Get a PFN for our page.
    /// </summary>
    /// <param name="PT">pointer to page table</param>
    /// <param name="DTLB">pointer to related data TLB</param>
    /// <param name="DC">pointer to related data cache</param>
    /// <param name="VPNindex">VPN of faulted page.</param>
    /// <returns>PFN for new page.</returns>
    int PageFaultHandler::HandleFault(PageTable* PT, TLB* DTLB, Cache* DC, int VPNindex) {
        int PFN = PageFaultHandler::FindFreeFrame(PT);            // Find free frame to use.
        if(PFN == -1) {                                           // Is there a free frame?
            PFN = PageFaultHandler::LRUReplacePage(PT, DTLB, DC); // If not, swap out a frame.
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
    int PageFaultHandler::FindFreeFrame(PageTable* PT) {
        return PT->AllocateFrame();
    }

    /// <summary>
    /// Runs LRU replacement for pages.
    /// </summary>
    /// <param name="PT">pointer to page table</param>
    /// <returns>PFN of victim entry.</returns>
    int PageFaultHandler::LRUReplacePage(PageTable* PT, TLB* DTLB, Cache* DC) {
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
        // Update DC
        int dirtyCount = DC->Invalidate(victimPFN);
        if (DC->GetPolicy() == 1) // write-back policy; update mem references
        {
            memoryReferences++;
        }

        return victimPFN;
    }