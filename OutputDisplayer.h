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

class OutputDisplayer {
private:
    MemoryOptions configOutput;                 // information about configuration
    std::vector<Trace> traceOutput;             // information about each trace
    std::vector<TraceStats> referencesOutput;   // information about each reference
    HardwareStats TLBOutput;       // information on hit, miss, rate for TLB
    HardwareStats PTOutput;        // information on hit, miss, rate for PT
    HardwareStats DCOutput;        // information on hit, miss, rate for DC
public:
    
    OutputDisplayer(/* args */);
    ~OutputDisplayer();
    
    /// PURPOSE: Set configOutput
    void FeedConfigOutput(MemoryOptions mem);

    /// PURPOSE: Add to traceOutput
    void AddTrace(Trace newTrace);
    
    /// PURPOSE: Add to referencesOutput
    void AddReferenceInfo(TraceStats referenceInfo); 

    /// PURPOSE: Set TLBOutput
    void FeedTLBStats(HardwareStats stats);

    /// PURPOSE: Set PTOutput
    void FeedPTStats(HardwareStats stats);

    /// PURPOSE: Set DCOutput
    void FeedDCStats(HardwareStats stats);

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
    this->referencesOutput.push_back(referenceInfo);
}

void OutputDisplayer::AddTrace(Trace newTrace) {
    this->traceOutput.push_back(newTrace);
}

#endif //OUTPUT_DISP_H