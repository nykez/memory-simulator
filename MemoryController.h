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
#include "BinaryConverter.h"
#include "PageFaultHandler.h"
#include "Trace.h"
#include "TraceStats.h"
#include "MemoryOptions.h"
#include "AddressStructs.h"
#include "HardwareStats.h"
#include <cmath>

class MemoryController {
private:
    //DTLB TLB;             // our TLB
    Cache *DC;             // our data cache
    PageTable PT;           // our page table

    MemoryOptions MemConfig;
    // Configuration
    bool useVirtualMemory;  // true: use PT and TLB. False: only use DC
    bool useTLB;            // true: use PT,TLB,DC. False: use PT, DC

    int bitCountOffset;     // number of bits for offset
    int bitCountVPN;        // number of bits for VPN
    int bitCountPFN;        // number of bits for PFN

public:
    /// Constructor: 
    MemoryController();
    MemoryController(MemoryOptions config);
    /// die
    ~MemoryController();

    // public interface for using controller.
    TraceStats RunMemory(Trace trace);
    int CheckPageTable(TraceStats* traceW);
    int CheckDataTLB(TraceStats* traceW);
    /// Calls pagefault handler
    int HandlePageFault(int VPN);
    void TranslateVirtualMemory(TraceStats* traceW);
    void AttachVPNandOffset(TraceStats* traceW);

    HardwareStats GetPTStats();
    HardwareStats GetDTLBStats();
    HardwareStats GetDCStats();
    MemoryOptions GetConfigOptions();


};




MemoryController::MemoryController() {
    PT = PageTable(64, 4, 256);
    bitCountOffset = (int)log2((double)256);
    bitCountPFN = (int)log2((double)4);
    bitCountVPN = (int)log2((double)4);
}

MemoryController::MemoryController(MemoryOptions config) {
    ///TODO: TLB

    // create data cache
    config.dcTotalSets = config.dcEntries / config.dcSetSize;     // calculate total sets

    // calculate index bits
    config.cacheIndexBits = log2(config.dcTotalSets);
    // calculate offset bits
    config.cacheOffsetBits = log2(config.dcLineSize);
    // calculate tag bits
    config.cacheTagBits = log2(config.pageSize) + log2(config.frameCount) - config.cacheIndexBits - config.cacheOffsetBits;

    cout << config.cacheTagBits << endl;

    DC = new Cache(config.dcTotalSets, config.dcEntries / config.dcTotalSets);

    // Determine bit counts
    bitCountOffset = (int)log2((double)config.pageSize);
    bitCountPFN = (int)log2((double)config.frameCount);
    bitCountVPN = (int)log2((double)config.pageCount);

    // create page table
    PT = PageTable(config.pageCount, config.frameCount, config.pageSize);

    // Configure controller
    useVirtualMemory = config.useVirt;
    if(useVirtualMemory == false) useTLB == false;
    else useTLB = config.useTLB;

    MemConfig = config;
}

MemoryController::~MemoryController() {
}

MemoryOptions MemoryController::GetConfigOptions()
{
    return MemConfig;
}

TraceStats MemoryController::RunMemory(Trace trace) {
    TraceStats traceW(trace);                // track trace events
    if(useVirtualMemory)                   // if we use virtual addresses
        TranslateVirtualMemory(&traceW);     // transform into physical address
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
int MemoryController::CheckDataTLB(TraceStats* traceW) {
    std::pair<bool, int> res;               // first: MISS/HIT(F/T). second: PFN
    res = {false, 0};                       // PLACEHOLDER for TLB
    if(res.first == false) {                // if TLB MISS
        traceW->TLBresult = "MISS";         // | log MISS
        return CheckPageTable(traceW);      // | return PFN from page table
    } else {                                // if TLB HIT
        traceW->TLBresult = "HIT";          // | log HIT
        return res.second;                  // | return PFN from TLB
    }
} 

/// PURPOSE: Check page table for PFN of a given VPN
int MemoryController::CheckPageTable(TraceStats* traceW) {
    std::pair<bool, int> res;               // first: MISS/HIT(F/T). second: PFN
    res = PT.TranslateVPN(traceW->VPN);     // check page table
    if(res.first == false) {                // if PT MISS
        traceW->PTresult = "MISS";          // | log MISS
        return HandlePageFault(traceW->VPN);// | return PFN from page fault handler
    } else {                                // if PT HIT
        traceW->PTresult = "HIT";           // | log HIT
        return res.second;                  // | return PFN from PT
    }
}

int MemoryController::HandlePageFault(int VPN) {
    //return PageFaultHandler::HandleFault(&PT, &DC, VPN);
    return PageFaultHandler::HandleFault(&PT, VPN);
}

HardwareStats MemoryController::GetPTStats() {
    HardwareStats PTstats(PT.GetHitCount(), PT.GetMissCount());
    return PTstats;
}


#endif