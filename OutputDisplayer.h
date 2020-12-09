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
    MemoryOptions configOutput;    // information about configuration
    HardwareStats TLBOutput;       // information on hit, miss, rate for TLB
    HardwareStats PTOutput;        // information on hit, miss, rate for PT
    HardwareStats DCOutput;        // information on hit, miss, rate for DC
    ReferenceStats miscOutput;     // what we touched
    int reads;                     // number of reads
    int writes;                    // number of writes
    double rwRatio;                // read-to-write ratio
    string policyStr = "";
public:
    
    OutputDisplayer(/* args */);
    ~OutputDisplayer();
    
    /// PURPOSE: Set configOutput
    void FeedConfigOutput(MemoryOptions mem);

    /// PURPOSE: Set TLBOutput
    void FeedTLBStats(HardwareStats stats);

    /// PURPOSE: Set PTOutput
    void FeedPTStats(HardwareStats stats);

    /// PURPOSE: Set DCOutput
    void FeedDCStats(HardwareStats stats);

    /// PURPOSE: Set miscOutput
    void FeedMiscOutput(ReferenceStats stats);

    /// PURPOSE: Display config info
    void DisplayConfig();

    /// PURPOSE: Display trace header
    void DisplayTraceHeader();

    /// PURPOSE: Display trace
    void DisplayTraceLine(TraceStats line);

    /// PURPOSE: Display hit/miss/rate of components
    void DisplayComponentStats();

    /// PURPOSE: Display various stats
    void DisplayMiscStats();

    /// PURPOSE: Set R/W info
    void FeedReadWriteInfo(int numReads, int numWrites);

    /// PURPOSE: Display read/write info
    void DisplayReadWriteInfo();
};



#endif //OUTPUT_DISP_H