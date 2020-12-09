/// NAME: MemoryControler
/// PURPOSE: Manages interactions between PT, TLB, and DC.
///         Including Evictions, control flow, and returns to Simulation.
/// AUTHOR: Harrison Lee Pollitte
/// CREATED: 12/6/2020
/// NOTES:
/// * Called by SimulationRunner
/// * Software Manager of our memory components.
/// * Linker between them all. Return most stuff to here.

#ifndef MEM_CON_H
#define MEM_CON_H
#include "DataCache/Cache.h"
#include "PageTable/PageTable.h"
#include "LookupBuffer/DTLB.h"
#include "ReferenceStats.h"
#include "PageFaultHandler.cpp"
#include "Trace.h"
#include "TraceStats.h"
#include "MemoryOptions.h"
#include "AddressStructs.h"
#include "HardwareStats.h"
#include <cmath>

int CreateBitMasking(int startBit, int endBit)
{
	int mask = 0;
	for (int i = startBit; i <= endBit; ++i) {
		mask |= 1 << i;
	}
	return mask;
}

class MemoryController {
private:
    TLB    *DTLB;           // our TLB
    Cache    *DC;           // our data cache
    PageTable*PT;           // our page table

    // Statistics
    int refCountMainMemory = 0; // number of references to main memory
    int refCountDisk       = 0; // number of references to external disk
    int refCountPageTable  = 0; // number of references to page table

    MemoryOptions MemConfig;
    // Configuration
    bool useVirtualMemory;  // true: use PT and TLB. False: only use DC
    bool useTLB;            // true: use PT,TLB,DC. False: use PT, DC

    int bitCountOffset;     // number of bits for offset
    int bitCountVPN;        // number of bits for VPN
    int bitCountPFN;        // number of bits for PFN



    int CheckPageTable(TraceStats* traceW);
    int CheckDataTLB(TraceStats* traceW);
    int HandlePageFault(int VPN);
    void TranslateVirtualMemory(TraceStats* traceW);
    void AttachVPNandOffset(TraceStats* traceW);
    int ExtractBits(int number, int k, int p);
    /// <summary>
    /// Combine PFN and offset into a physical address.
    /// </summary>
    /// <param name="PFN">the Physical Frame Number.</param>
    /// <param name="offset">the offset into the frame.</param>
    /// <returns>the physical address.</returns>
    int CalculatePhysicalAddress(int PFN, int offset);
public:
    /// Constructor: 
    MemoryController();
    MemoryController(MemoryOptions config);
    /// die
    ~MemoryController();

    // public interface for using controller.
    TraceStats RunMemory(Trace trace);
    HardwareStats GetPTStats();
    HardwareStats GetDTLBStats();
    HardwareStats GetDCStats();
    MemoryOptions GetConfigOptions();

    /// <summary>
    /// Get number of references to main memory during simulation.
    /// Includes:
    /// * Page table accesses
    /// * 
    /// </summary>
    /// <returns>number of references to main memory.</returns>
    int GetReferenceCountToMemory();

    /// <summary>
    /// Get number of references to disk during simulation.
    /// Includes:
    /// * Page faults
    /// </summary>
    /// <returns>number of references to disk.</returns>
    int GetReferenceCountToDisk();

    /// <summary>
    /// Get number of references to page table during simulation.
    /// Includes:
    /// * Page table accesses.
    /// </summary>
    /// <returns>number of references to page table.</returns>    
    int GetReferenceCountToPageTable();


    /// Adds to the total count of memory references
    void AddReferenceCountToMemory(int amountToAdd);

    ReferenceStats GetReferenceCounts();


    void SetupConfigBits(MemoryOptions* MO);
};


void MemoryController::SetupConfigBits(MemoryOptions* config) {
    // Internal use
    bitCountOffset  = (int)log2((double)config->pageSize);
    bitCountPFN     = (int)log2((double)config->frameCount);
    bitCountVPN     = (int)log2((double)config->pageCount);
    

    // Configure data cache
    config->dcTotalSets = config->dcEntries / config->dcSetSize;     // calculate total sets
    config->cacheIndexBits = log2(config->dcTotalSets);             // calculate index bits
    config->cacheOffsetBits = log2(config->dcLineSize);             // calculate offset bits
    config->cacheTagBits = log2(config->pageSize) +                 // calculate tag bits
                          log2(config->frameCount) - 
                          config->cacheIndexBits - 
                          config->cacheOffsetBits;
    config->cacheEntriesPerSet = config->dcEntries / config->dcTotalSets;  // entries per set


    // For return
    config->vpnBits = bitCountVPN;
    config->pfnBits = bitCountPFN;
    config->offBits = bitCountOffset;
}



MemoryController::MemoryController() {
    SetupConfigBits(&MemConfig);
}



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

MemoryController::~MemoryController() {
    //delete DC;
}

MemoryOptions MemoryController::GetConfigOptions() {
    return MemConfig;
}



/// PURPOSE: Runs memory simulation
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
    int index = (physicalAddress & CreateBitMasking(MemConfig.cacheOffsetBits, MemConfig.cacheOffsetBits + MemConfig.cacheIndexBits -1)) >> MemConfig.cacheOffsetBits;
    // get cache tag
    int otherTag = (physicalAddress & CreateBitMasking(MemConfig.cacheOffsetBits + MemConfig.cacheIndexBits, 31)) >> (MemConfig.cacheOffsetBits + MemConfig.cacheIndexBits);
    
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

/// PURPOSE: Convert virtual addr to physical addr
void MemoryController::TranslateVirtualMemory(TraceStats* traceW) {
    AttachVPNandOffset(traceW);         //Add VPN and pageOffset to traceW
    int PFN = -1;  
    if(useTLB) {                  // if we use DTLB
        PFN = CheckDataTLB(traceW);     // check DTLB first
    } else {                            // if we don't use DTLB
        PFN = CheckPageTable(traceW);   // only check page table
    }


    traceW->PFN = PFN;                  // assign PFN
}

/// PURPOSE: Generate and attach VPN and offset to a trace
void MemoryController::AttachVPNandOffset(TraceStats* traceW) {
    VirtualAddress virtAddr(traceW->trace.hexAddress, bitCountOffset);
    //NOTE: We can make trace hold a bit-array if necessary. We have to change this tho (it isn't hard to change).
    traceW->VPN         = (((1 << bitCountVPN) - 1) & (traceW->trace.hexAddress >> bitCountOffset));
    traceW->pageOffset  = (((1 << bitCountOffset) - 1) & (traceW->trace.hexAddress));
}

/// PURPOSE: Check DTLB for PFN of given VPN
/// RETURNS: PFN
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

/// PURPOSE: Check page table for PFN of a given VPN
/// RETURNS: PFN
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

/// PURPOSE: Handle a page fault
/// RETURNS: PFN
int MemoryController::HandlePageFault(int VPN) {
    return PageFaultHandler::HandleFault(PT, DTLB, DC, VPN);
}

// add to the main memory touches
void MemoryController::AddReferenceCountToMemory(int amountToAdd)
{
    refCountMainMemory = refCountMainMemory + amountToAdd;
}

/// PURPOSE: Get stats of page table
/// RETURNS: HardwareStats of page table
HardwareStats MemoryController::GetPTStats() {
    return PT->GetStatistics();
}

HardwareStats MemoryController::GetDTLBStats() {
    return DTLB->GetStatistics();
}

HardwareStats MemoryController::GetDCStats()
{
    return DC->GetStatistics();
}


int MemoryController::CalculatePhysicalAddress(int PFN, int offset) {
    return (PFN << bitCountOffset) | offset;
}

int MemoryController::ExtractBits(int number, int k, int p)
{
    return (((1 << k) - 1) & (number >> (p - 1))); 
}

ReferenceStats MemoryController::GetReferenceCounts() {
    ReferenceStats refStats(refCountPageTable, refCountMainMemory + PageFaultHandler::memoryReferences, refCountDisk);
    return refStats;
}


#endif