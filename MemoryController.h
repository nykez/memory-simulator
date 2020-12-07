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
#include <cmath>

class MemoryController {
private:
    //DTLB TLB;             // our TLB
    Cache::Cache DC;        // our data cache
    PageTable PT;           // our page table
    // Configuration
    bool useVirtualMemory; // true: use PT and TLB. False: only use DC
    bool useTLB;            // true: use PT,TLB,DC. False: use PT, DC

    int bitCountOffset;
    int bitCountVPN;
    int bitCountPFN;

public:
    /// Constructor: 
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

};

MemoryController::MemoryController(MemoryOptions config) {
    ///TODO: TLB
    DC = Cache(config.dcEntries, config.dcSetSize);
    PT = PageTable(config.pageCount, config.frameCount, config.pageSize);

    // Determine bit counts
    bitCountOffset = (int)log2((double)config.pageSize);
    bitCountPFN = (int)log2((double)config.frameCount);
    bitCountPFN = (int)log2((double)config.pageCount);

    // Configure controller
    this->useVirtualMemory = config.useVirt;
    if(this->useVirtualMemory == false) this->useTLB == false;
    else this->useTLB = config.useTLB;
}

MemoryController::~MemoryController() {
}



TraceStats MemoryController::RunMemory(Trace trace) {
    TraceStats traceW(trace);                // track trace events
    if(this->useVirtualMemory)                   // if we use virtual addresses
        TranslateVirtualMemory(&traceW);     // transform into physical address
    return traceW;
}

/// PURPOSE: Convert virtual addr to physical addr
void MemoryController::TranslateVirtualMemory(TraceStats* traceW) {
    AttachVPNandOffset(traceW);         //Add VPN and pageOffset to traceW
    int PFN = -1;  
    if(this->useTLB) {                  // if we use DTLB
        PFN = CheckDataTLB(traceW);     // check DTLB first
    } else {                            // if we don't use DTLB
        PFN = CheckPageTable(traceW);   // only check page table
    }

}

/// PURPOSE: Generate and attach VPN and offset to a trace
void MemoryController::AttachVPNandOffset(TraceStats* traceW) {
    VirtualAddress virtAddr(traceW->trace.hexAddress, this->bitCountOffset);
    //NOTE: We can make trace hold a bit-array if necessary. We have to change this tho (it isn't hard to change).
    traceW->VPN         = BinaryConverter::ToBinaryInt(virtAddr.VPN);
    traceW->pageOffset  = BinaryConverter::ToBinaryInt(virtAddr.offset);
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
    PageFaultHandler::HandleFault(&PT, &DC, VPN);
}
#endif