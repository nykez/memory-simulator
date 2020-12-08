/// NAME: OutputDisplayer.h
/// PURPOSE: Display output to console
/// AUTHOR: Harrison Lee Pollitte
/// CREATED: 12/6/2020
/// NOTES:
///
///

#ifndef OUTPUT_DISP_H
#define OUTPUT_DISP_H

#include <vector>
#include "TraceStats.h"
#include "MemoryOptions.h"
#include "HardwareStats.h"
#include "ReferenceStats.h"
class OutputDisplayer {
private:
    MemoryOptions configOutput;            // information about configuration
    std::vector<TraceStats> traceOutput;   // information about each reference
    HardwareStats TLBOutput;       // information on hit, miss, rate for TLB
    HardwareStats PTOutput;        // information on hit, miss, rate for PT
    HardwareStats DCOutput;        // information on hit, miss, rate for DC
    ReferenceStats miscOutput;     // what we touched
public:
    
    OutputDisplayer(/* args */);
    ~OutputDisplayer();
    
    /// PURPOSE: Set configOutput
    void FeedConfigOutput(MemoryOptions mem);

    /// PURPOSE: Add to traceOutput
    void AddReferenceInfo(TraceStats referenceInfo); 

    /// PURPOSE: Set TLBOutput
    void FeedTLBStats(HardwareStats stats);

    /// PURPOSE: Set PTOutput
    void FeedPTStats(HardwareStats stats);

    /// PURPOSE: Set DCOutput
    void FeedDCStats(HardwareStats stats);

    /// PURPOSE: Set miscOutput
    void FeedMiscOutput(ReferenceStats stats);

    /// PURPOSE: Public method to display
    void DisplayAll();

    /// PURPOSE: Display config info
    void DisplayConfig();

    /// PURPOSE: Display traces
    void DisplayTraces();

    /// PURPOSE: Display lines of trace info
    void DisplayReferenceInfo();

    /// PURPOSE: Display hit/miss/rate of components
    void DisplayComponentStats();

    /// PURPOSE: Display various stats
    void DisplayMiscStats();
};

OutputDisplayer::OutputDisplayer(/* args */) {
    
}

OutputDisplayer::~OutputDisplayer() {

}

#pragma region input-methods

void OutputDisplayer::FeedConfigOutput(MemoryOptions mem) {
    this->configOutput = mem;
}

void OutputDisplayer::FeedTLBStats(HardwareStats stats) {
    this->TLBOutput = stats;
}

void OutputDisplayer::FeedPTStats(HardwareStats stats) {
    this->PTOutput = stats;
}

void OutputDisplayer::FeedDCStats(HardwareStats stats) {
    this->DCOutput = stats;
}

void OutputDisplayer::AddReferenceInfo(TraceStats referenceInfo) {
    this->traceOutput.push_back(referenceInfo);
}

void OutputDisplayer::FeedMiscOutput(ReferenceStats stats) {
    this->miscOutput = stats;
}

#pragma endregion

#pragma region Display-methods
void OutputDisplayer::DisplayAll() {
    DisplayConfig();
    DisplayReferenceInfo();
    DisplayComponentStats();
    DisplayMiscStats();
}

void OutputDisplayer::DisplayConfig() {
    printf("Data TLB contains %d entries.\n", configOutput.tlbEntries);
    printf("\n");
    printf("Number of virtual pages is %d.\n", configOutput.pageCount);
    printf("Number of physical frames is %d.\n", configOutput.frameCount);
    printf("Each page contains %d bytes.\n", configOutput.pageSize);
    printf("Number of bits for page-table index is [PLACEHOLDER].\n");
    printf("Number of bits for page offset is [PLACEHOLDER].\n");
    printf("\n");
    printf("Data cache contains %d sets.\n", configOutput.dcTotalSets);
    printf("Each set contains %d entries.\n", configOutput.dcEntries);
    printf("Each line is %d bytes.\n", configOutput.dcLineSize);
    printf("The set uses a [PLACEHOLDER] policy\n");
    printf("Number of bits used for the tag is [PLACEHOLDER].\n");
    printf("Number of bits used for the index  is [PLACEHOLDER].\n");
    printf("Number of bits used for the offset is [PLACEHOLDER].\n");
    printf("\n");
    if(configOutput.useVirt) {
        printf("The addresses read in are VIRTUAL addresses.\n");
    } else {
        printf("The addresses read in are PHYSICAL addresses.\n");
    }
    if(configOutput.useTLB) {
        printf("The translation lookaside buffer is ENABLED.\n");
    } else {
        printf("The translation lookaside buffer is DISABLED.\n");
    }
    printf("\n");
}

void OutputDisplayer::DisplayReferenceInfo() {
    printf("Virtual  Virt.  Page TLB  PT   Phys    DC  DC      \n");
    printf("Address  Page # Off. Res. Res. Pg # DC Tag Idx Res.\n");
    printf("-------- ------ ---- ---- ---- ---- ------ --- ----\n");
    for(int i = 0; i < this->traceOutput.size(); i++) {
        printf("%08x %6x %4x %4s %4s %4x %6x %3x %4s\n", 
                 traceOutput[i].trace.hexAddress,
                 traceOutput[i].VPN,
                 traceOutput[i].pageOffset,
                 traceOutput[i].TLBresult.c_str(),
                 traceOutput[i].PTresult.c_str(),
                 traceOutput[i].PFN,
                 traceOutput[i].DCtag,
                 traceOutput[i].DCidx,
                 traceOutput[i].DCresult.c_str());
    }
}

void OutputDisplayer::DisplayComponentStats() {
    printf("\n\nSimulation Statistics\n");
    printf("---------------------\n");
    printf("\n");
    printf("Data TLB hits        : %d\n", TLBOutput.hits);
    printf("Data TLB misses      : %d\n", TLBOutput.misses);
    printf("Data TLB hit ratio   : %f\n", TLBOutput.hitrate);
    printf("\n");
    printf("Page Table hits      : %d\n", PTOutput.hits);
    printf("Page Table misses    : %d\n", PTOutput.misses);
    printf("Page Table hit ratio : %f\n", PTOutput.hitrate);
    printf("\n");
    printf("Data Cache hits      : %d\n", DCOutput.hits);
    printf("Data Cache misses    : %d\n", DCOutput.misses);
    printf("Data Cache hit ratio : %f\n", DCOutput.hitrate);
    printf("\n");

}

void OutputDisplayer::DisplayMiscStats() {
    printf("Main Memory references: %d\n",miscOutput.mainMemoryRefCount);
    printf("Page Table  references: %d\n",miscOutput.pageTableRefCount);
    printf("Disk References       : %d\n",miscOutput.diskRefCount);
}
#pragma endregion

#endif //OUTPUT_DISP_H