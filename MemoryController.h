

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
#include "BitUtilities.cpp"
#include "DataCache/Cache.h"
#include "PageTable/PageTable.cpp"
#include "LookupBuffer/DTLB.cpp"
#include "ReferenceStats.h"
#include "PageFaultHandler.cpp"
#include "Trace.h"
#include "TraceStats.h"
#include "MemoryOptions.h"
#include "HardwareStats.h"
#include <cmath>


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
    int CalculatePhysicalAddress(int PFN, int offset);
    void SetupConfigBits(MemoryOptions* MO);
public: 
    MemoryController();
    MemoryController(MemoryOptions config);
    ~MemoryController();
    TraceStats RunMemory(Trace trace);
    HardwareStats GetPTStats();
    HardwareStats GetDTLBStats();
    HardwareStats GetDCStats();
    MemoryOptions GetConfigOptions();
    ReferenceStats GetReferenceCounts();
};



#endif