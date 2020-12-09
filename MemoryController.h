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



#endif