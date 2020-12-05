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

#pragma once

#include "PageTable.h"
#include "PageTableEntry.h"
#include "BinaryConverter.h"

#include <vector>
#include <iostream>

namespace PageFaultHandler {
    void HandleFault(PageTable* PT, int VPNindex) {
        int PFN = FindFreeFrame(PT);    // Find free frame to use.
        if(PFN == -1) {                 // Is there a free frame?
            PFN = LRUReplacePage(PT);   // If not, swap out a frame.
            //[TODO]: Update TLB
            //[TODO]: Update DC
        }
        // Update entry's PFN
        std::vector<int> retPFN = BinaryConverter::ToBitArray(PFN);
        PT->entries.at(VPNindex).PFN = retPFN;
    }

    /// <summary>
    /// Returns PFN of free frame (If any).
    /// </summary>
    /// <param name="PT">pointer to page table</param>
    /// <returns>PFN of free frame. -1 if no free frames.</returns>
    int FindFreeFrame(PageTable* PT) {
       if(PT->GetFrameCount() < PT->GetMaxFrameCount()) {
           return PT->GetFrameCount();
       }
       return -1;
    }

    int LRUReplacePage(PageTable* PT) {

    }
} // namespace PageFaultHandler

