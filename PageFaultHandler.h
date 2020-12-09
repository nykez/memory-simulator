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

    int static memoryReferences = 0;

    int HandleFault(PageTable* PT, TLB* DTLB, Cache* DC, int VPNindex);
    int FindFreeFrame(PageTable* PT);
    int LRUReplacePage(PageTable* PT, TLB* DTLB, Cache* DC);


} // namespace PageFaultHandler

#endif