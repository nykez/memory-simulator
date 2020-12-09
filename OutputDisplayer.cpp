/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Project:		GroupProject
//	File Name:		OutputDisplayer.cpp
//	Description:    Implementation of OutputDisplayer.h
//	Course:			CSCI-4727-940: Operating Systems
//	Author:			Harrison Pollitte, pollitteh@etsu.edu, Department of Computing, East Tennessee State University
//	Created:		Saturday, December 5 2020
//	Copyright:	    Harrison Pollitte 2020
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "OutputDisplayer.h"

/// <summary>
/// Default constructor.
/// </summary>
OutputDisplayer::OutputDisplayer(/* args */) {
    
}

/// <summary>
/// Deconstructor.
/// </summary>
OutputDisplayer::~OutputDisplayer() {

}

#pragma region input-methods
/// <summary>
/// Give configuration to display later.
/// </summary>
/// <param name="mem">memory congfig to display</param>
void OutputDisplayer::FeedConfigOutput(MemoryOptions mem) {
    this->configOutput = mem;
    if(configOutput.dcPolicy == 0) {
        policyStr = "Write-Through";
    }
    else {
        policyStr = "Write-Back";
    }
}

/// <summary>
/// Give TLB stats to display later.
/// </summary>
/// <param name="stats">TLB stats to display</param>
void OutputDisplayer::FeedTLBStats(HardwareStats stats) {
    this->TLBOutput = stats;
}

/// <summary>
/// Give pT stats to display later.
/// </summary>
/// <param name="stats">PT stats to display</param>
void OutputDisplayer::FeedPTStats(HardwareStats stats) {
    this->PTOutput = stats;
}

/// <summary>
/// Give DC stats to display later.
/// </summary>
/// <param name="stats">DC stats to display</param>
void OutputDisplayer::FeedDCStats(HardwareStats stats) {
    this->DCOutput = stats;
}

/// <summary>
/// Give reference stats to display later.
/// </summary>
/// <param name="stats">stats to display.</param>
void OutputDisplayer::FeedReferenceOutput(ReferenceStats stats) {
    this->miscOutput = stats;
}

/// <summary>
/// Give read/write info to display later.
/// </summary>
/// <param name="numReads">number of reads.</param>
/// <param name="numWrites">number of writes.</param>
void OutputDisplayer::FeedReadWriteInfo(int numReads, int numWrites) {
    reads = numReads;
    writes = numWrites;
    rwRatio = (double) numReads / (double)(numReads + numWrites);
}

#pragma endregion

#pragma region Display-methods

/// <summary>
/// Display configuration file
/// </summary>
void OutputDisplayer::DisplayConfig() {
    printf("Data TLB contains %d entries.\n", configOutput.tlbEntries);
    printf("\n");
    printf("Number of virtual pages is %d.\n", configOutput.pageCount);
    printf("Number of physical frames is %d.\n", configOutput.frameCount);
    printf("Each page contains %d bytes.\n", configOutput.pageSize);
    printf("Number of bits for page-table index is %d.\n", configOutput.vpnBits);
    printf("Number of bits for page offset is %d.\n", configOutput.offBits);
    printf("\n");
    printf("Data cache contains %d sets.\n", configOutput.dcTotalSets);
    printf("Each set contains %d entries.\n", configOutput.cacheEntriesPerSet);
    printf("Each line is %d bytes.\n", configOutput.dcLineSize);
    printf("The set uses a %s policy\n", policyStr.c_str());
    printf("Number of bits used for the tag is %d.\n", configOutput.cacheTagBits);
    printf("Number of bits used for the index  is %d.\n", configOutput.cacheIndexBits);
    printf("Number of bits used for the offset is %d.\n", configOutput.cacheOffsetBits);
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

/// <summary>
/// Display trace header.
/// </summary>
void OutputDisplayer::DisplayTraceHeader() {
    printf("Virtual  Virt.  Page TLB  PT   Phys        DC  DC  \n");
    printf("Address  Page # Off. Res. Res. Pg # DC Tag Idx Res.\n");
    printf("-------- ------ ---- ---- ---- ---- ------ --- ----\n");
}

/// <summary>
/// Display trace.
/// </summary>
/// <param name="line">trace to display</param>
void OutputDisplayer::DisplayTraceLine(TraceStats line) {
    printf("%08x %6x %4x %4s %4s %4x %6x %3x %4s\n", 
                 line.trace.hexAddress,
                 line.VPN,
                 line.pageOffset,
                 line.TLBresult.c_str(),
                 line.PTresult.c_str(),
                 line.PFN,
                 line.DCtag,
                 line.DCidx,
                 line.DCresult.c_str());
}

/// <summary>
/// Display component stats.
/// </summary>
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

/// <summary>
/// Display read/write info
/// </summary>
void OutputDisplayer::DisplayReadWriteInfo() {
    printf("Total reads          : %d\n", reads);
    printf("Total writes         : %d\n", writes);
    printf("Ratio of reads       : %f\n", rwRatio);
}

/// <summary>
/// Display reference info
/// </summary>
void OutputDisplayer::DisplayReferenceStats() {
    printf("Main Memory references: %d\n",miscOutput.mainMemoryRefCount);
    printf("Page Table  references: %d\n",miscOutput.pageTableRefCount);
    printf("Disk References       : %d\n",miscOutput.diskRefCount);
}
#pragma endregion