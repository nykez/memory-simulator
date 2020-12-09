/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Project:		GroupProject
//	File Name:		OutputDisplayer.h
//	Description:    Display output to console
//	Course:			CSCI-4727-940: Operating Systems
//	Author:			Harrison Pollitte, pollitteh@etsu.edu, Department of Computing, East Tennessee State University
//	Created:		Saturday, December 5 2020
//	Copyright:	    Harrison Pollitte 2020
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef OUTPUT_DISP_H
#define OUTPUT_DISP_H

#include <vector>
#include "TraceStats.h"
#include "MemoryOptions.h"
#include "HardwareStats.h"
#include "ReferenceStats.h"
///<summary>
/// display stuff
///</summary>
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
    std::string policyStr = "";
public:
    
    OutputDisplayer(/* args */);
    ~OutputDisplayer();
    
    void FeedConfigOutput(MemoryOptions mem);
    void FeedTLBStats(HardwareStats stats);
    void FeedPTStats(HardwareStats stats);
    void FeedDCStats(HardwareStats stats);
    void FeedReferenceOutput(ReferenceStats stats);
    void DisplayConfig();
    void DisplayTraceHeader();
    void DisplayTraceLine(TraceStats line);
    void DisplayComponentStats();
    void DisplayReferenceStats();
    void FeedReadWriteInfo(int numReads, int numWrites);
    void DisplayReadWriteInfo();
};
#endif //OUTPUT_DISP_H