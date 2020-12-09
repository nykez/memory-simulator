//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Project:		GroupProject
//	File Name:		MemoryController.cpp
//	Description:    Implementation of MemoryController.h
//	Course:			CSCI-4727-940: Operating Systems
//	Author:			Harrison Pollitte, pollitteh@etsu.edu, Department of Computing, East Tennessee State University
//	Created:		Sunday, December 6 2020
//	Copyright:	    Harrison Pollitte 2020
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MemoryController.h"
#include "BitUtilities.h"
#include "TableEntry.h"

/// <summary>
/// Setup the passed configuration, filling in missing data.
/// </summary>
/// <param name="config">pointer to memory option</param>
void MemoryController::SetupConfigBits(MemoryOptions* config) {
    // Internal use
    bitCountOffset  = (int)log2((double)config->pageSize);
    bitCountPFN     = (int)log2((double)config->frameCount);
    bitCountVPN     = (int)log2((double)config->pageCount);
    

    // Configure data cache
    config->dcTotalSets = config->dcEntries / config->dcSetSize;     // calculate total sets
    config->cacheIndexBits = log2(config->dcTotalSets);             // calculate index bits
    config->cacheOffsetBits = log2(config->dcLineSize);             // calculate offset bits
    config->cacheTagBits = log2(config->pageSize) + (config->frameCount) - config->cacheIndexBits - config->cacheOffsetBits;
    config->cacheEntriesPerSet = config->dcEntries / config->dcTotalSets;  // entries per set

    // For return
    config->vpnBits = bitCountVPN;
    config->pfnBits = bitCountPFN;
    config->offBits = bitCountOffset;
}


/// <summary>
/// Default constructor
/// </summary>
MemoryController::MemoryController() {
    SetupConfigBits(&MemConfig);
}


/// <summary>
/// Parameterized constructor. Give initial config.
/// </summary>
/// <param name="config">initial memoryoptions</param>
MemoryController::MemoryController(MemoryOptions config) {
    SetupConfigBits(&config);

    // Configure TLB
    DTLB = new TLB(config.tlbEntries);
    
    // create data cache
    DC = new Cache(config.dcTotalSets, config.cacheEntriesPerSet);
    DC->SetPolicy(config.dcPolicy);

    // create page table
    PT = new PageTable(config.pageCount, config.frameCount, config.pageSize);

    // Configure controller
    useVirtualMemory = config.useVirt;
    if(useVirtualMemory == false) useTLB == false;
    else useTLB = config.useTLB;

    MemConfig = config;
}

/// <summary>
/// Deconstructor
/// </summary>
MemoryController::~MemoryController() {
}

/// <summary>
/// Get configuration options.
/// </summary>
/// <returns>config options.</returns>
MemoryOptions MemoryController::GetConfigOptions() {
    return MemConfig;
}


/// <summary>
/// Run memory simulation
/// </summary>
/// <param name="trace">a trace.</param>
/// <returns>effects on the trace inputted.</returns>
TraceStats MemoryController::RunMemory(Trace trace) {
    TraceStats traceW(trace);                // track trace events
    if(useVirtualMemory)                     // if we use virtual addresses
        TranslateVirtualMemory(&traceW);     // transform into physical address
    else {                                   // if we don't use virtual address, get PFN and offset
        traceW.PFN = (((1 << bitCountPFN) - 1) & (traceW.trace.hexAddress >> bitCountOffset));
        traceW.pageOffset  = (((1 << bitCountOffset) - 1) & (traceW.trace.hexAddress));
    }
    // run data cache things
    // get physical address
    int physicalAddress = CalculatePhysicalAddress(traceW.PFN, traceW.pageOffset);
    // get cache index
    int index = (physicalAddress & BitUtilities::CreateBitMasking(MemConfig.cacheOffsetBits, MemConfig.cacheOffsetBits + MemConfig.cacheIndexBits -1)) >> MemConfig.cacheOffsetBits;
    // get cache tag
    int otherTag = (physicalAddress & BitUtilities::CreateBitMasking(MemConfig.cacheOffsetBits + MemConfig.cacheIndexBits, 31)) >> (MemConfig.cacheOffsetBits + MemConfig.cacheIndexBits);
    
    traceW.DCtag = otherTag;
    traceW.DCidx = index;

    int result = DC->GetEntry(index, otherTag);

    // hit
    if (result)
    {
        traceW.DCresult = "HIT";    // set trace output to HIT
        
        // add hit
        DC->hits++;

        // write through, no write allocate
        if (MemConfig.dcPolicy == 0)
        {

            if (trace.accessType == AccessType::Write)
            {
                // touched memory, update counter
                refCountMainMemory++;
            }
        }
        // write-back, write allocate
        else
        {
            if (trace.accessType == AccessType::Write)
            {
                // update cache (set dirty bit)
                DC->UpdateDirtyEntry(index, otherTag);
            }
        }
    }
    else
    {
        // set trace output to miss
        traceW.DCresult = "MISS";

        // add DC miss
        DC->misses++;

        // write through, no write allocate
        if (MemConfig.dcPolicy == 0)
        {
            if (trace.accessType == AccessType::Write)
            {
                // hit memory + update counter
                ++refCountMainMemory;
            }
            else
            {
                // bring in from "memory", update cache
                ++refCountMainMemory;
                // index, tag, dirtybit, PFN
                DC->AddEntry(index, otherTag, 0, traceW.PFN);
            }
        }
        // write-back, write allocate
        else
        {
            if (trace.accessType == AccessType::Write)
            {
                // Is set dirty?
                bool IsDirtyBit = DC->LRU_IsEntryDirtyBit(index);
                // UPDATE cache with new entry (it's  dirty because this is a write)
                DC->AddEntry(index, otherTag, 1, traceW.PFN);
                ++refCountMainMemory; // "touch/bringin from memory"

                // if replaced cache was dirty then we updated memory
                if (IsDirtyBit)
                {
                    // so update memory counter
                    ++refCountMainMemory;
                }

            }
            else
            {
                // bring in from "memory", update cache
                ++refCountMainMemory;
                // index, tag, dirtybit, PFN
                DC->AddEntry(index, otherTag, 0, traceW.PFN); // update the cache
            }
        }
        

    }

    return traceW;
}

/// <summary>
/// Get PFN, given a virtual address.
/// </summary>
/// <param name="traceW">pointer to a wrapped trace</param>
void MemoryController::TranslateVirtualMemory(TraceStats* traceW) {
    AttachVPNandOffset(traceW);         //Add VPN and pageOffset to traceW
    int PFN = -1;  
    if(useTLB) {                        // if we use DTLB
        PFN = CheckDataTLB(traceW);     // check DTLB first
    } else {                            // if we don't use DTLB
        PFN = CheckPageTable(traceW);   // only check page table
    }

    traceW->PFN = PFN;                  // assign PFN
}

/// <summary>
/// Generate and attach VPN and offset to a trace
/// </summary>
/// <param name="traceW">pointer to a wrapped trace.</param>
void MemoryController::AttachVPNandOffset(TraceStats* traceW) {
    traceW->VPN         = BitUtilities::ExtractBits(traceW->trace.hexAddress, bitCountVPN, bitCountOffset);
    traceW->pageOffset  = BitUtilities::ExtractBits(traceW->trace.hexAddress, bitCountOffset, 0);
}

/// <summary>
/// Check DTLB for PFN of a given VPN.
/// </summary>
/// <param name="traceW">pointer to a wrapped trace.</param>
/// <returns>PFN</returns>
int MemoryController::CheckDataTLB(TraceStats* traceW) {
    std::pair<bool, int> res;               // first: MISS/HIT(F/T). second: PFN
    res = DTLB->LookUp(traceW->VPN);         // check TLB
    if(res.first == false) {                // if TLB MISS
        traceW->TLBresult = "MISS";         // | log MISS
        res.second = CheckPageTable(traceW);// | return PFN from page table
        int dirtyBit = PT->GetEntryDirty(traceW->VPN);
        int validBit = PT->GetEntryValidity(traceW->VPN);
        int accessOrd= PT->GetEntryAccessOrdinal(traceW->VPN);
        TableEntry te(res.second, dirtyBit, validBit, accessOrd);
        DTLB->InsertEntry(traceW->VPN, te);  // | update TLB
    } else {                                // if TLB HIT
        traceW->TLBresult = "HIT";          // | log HIT
    }
    return res.second;                      // return PFN from TLB
} 

/// <summary>
/// Check page table for PFN of a given VPN
/// </summary>
/// <param name="traceW">pointer to a wrapped trace.</param>
/// <returns>PFN</returns>
int MemoryController::CheckPageTable(TraceStats* traceW) {
    refCountPageTable++;                    // we touch page table... 
    std::pair<bool, int> res;               // first: MISS/HIT(F/T). second: PFN
    res = PT->LookUp(traceW->VPN);           // check page table
    if(res.first == false) {                // if PT MISS
        refCountDisk++;                     // | go to disk
        traceW->PTresult = "MISS";          // | log MISS
        return HandlePageFault(traceW->VPN);// | return PFN from page fault handler
    } else {                                // if PT HIT
        traceW->PTresult = "HIT";           // | log HIT
        return res.second;                  // | return PFN from PT
    }
}

/// <summary>
/// Handle a page fault.
/// </summary>
/// <param name="">VPN of page that faulted.</param>
/// <returns>PFN</returns>
int MemoryController::HandlePageFault(int VPN) {
    return PageFaultHandler::HandleFault(PT, DTLB, DC, VPN);
}

/// <summary>
/// Get stats for page table.
/// </summary>
/// <returns>stats for page table.</returns>
HardwareStats MemoryController::GetPTStats() {
    return PT->GetStatistics();
}

/// <summary>
/// Get stats for Data TLB.
/// </summary>
/// <returns>stats for data tlb.</returns>
HardwareStats MemoryController::GetDTLBStats() {
    return DTLB->GetStatistics();
}

/// <summary>
/// Get stats for Data Cache.
/// </summary>
/// <returns>Stats for data cache</returns>
HardwareStats MemoryController::GetDCStats()
{
    return DC->GetStatistics();
}


/// <summary>
/// Combine PFN and offset to get physical address.
/// </summary>
/// <param name="PFN">bits of PFN</param>
/// <param name="offset">bits of offset</param>
/// <returns>the physical address</returns>
int MemoryController::CalculatePhysicalAddress(int PFN, int offset) {
    return (PFN << bitCountOffset) | offset;
}

/// <summary>
/// Get reference counts to various parts of machine.
/// </summary>
/// <returns>struct containing reference counts</returns>
ReferenceStats MemoryController::GetReferenceCounts() {
    ReferenceStats refStats(refCountPageTable, refCountMainMemory + PageFaultHandler::memoryReferences, refCountDisk);
    return refStats;
}



