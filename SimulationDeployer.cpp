//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Project:		GroupProject
//	File Name:		SimulationDeployer.cpp
//	Description:    Implementation of SimulationDeployer.h
//	Course:			CSCI-4727-940: Operating Systems
//	Author:			Harrison Pollitte, pollitteh@etsu.edu, Department of Computing, East Tennessee State University
//	Created:		Monday, December 7 2020
//	Copyright:	    Harrison Pollitte 2020
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "SimulationDeployer.h"

/// <summary>
/// Deault construtor
/// </summary>
SimulationDeployer::SimulationDeployer() {
}


/// <summary>
/// Deconstructor
/// </summary>
SimulationDeployer::~SimulationDeployer() {
}


/// <summary>
/// Initialize our simulation deployer.
/// Read in config file, parse it, setup our MemoryOption
/// setup our Memory Controller, display config and trace header
/// </summary>
void SimulationDeployer::Initialize(const string& configFile, const string& traceFile) {
    // Read config
    ///DAVID: READ IN MAP HERE inputReader.ReadConfigFile(configFile);
    auto options = inputReader.ReadConfigFile(configFile); //read in config file map
    // Setup config
    ///DAVID: Search through map, assinging things to our MO
    MO.tlbEntries = stoi(options.at("Number of entries"));
	MO.pageCount = stoi(options.at("Number of virtual pages"));
	MO.frameCount = stoi(options.at("Number of physical pages"));
	MO.pageSize = stoi(options.at("Page size"));
	MO.dcEntries = stoi(options.at("Number of entries"));
	MO.dcSetSize = stoi(options.at("Set size"));
	MO.dcLineSize = stoi(options.at("Line size"));
	string isWriteConf = options.at("Write through/no write allocate");
    if (isWriteConf == "N" || isWriteConf == "n")
    {
	    //TODO: use write through/no write memory option
    }
	string isVirtConf = options.at("Virtual addresses");
	if (isVirtConf == "n" || isVirtConf == "n")
    {
	    MO.useVirt = false;
    }
	string isTLBConf = options.at("TLB");
	if (isTLBConf == "N" || isTLBConf == "n")
    {
	    MO.useTLB = false;
    }
    // Setup Memory Controller
    MC = (MO);
    // Display config
    outputDisplayer.FeedConfigOutput(MC.GetConfigOptions());
    outputDisplayer.DisplayConfig();
    // Setup trace file
    outputDisplayer.DisplayTraceHeader();
    inputReader.SetTraceFile(traceFile);
}

///<summary>
/// Directs inputReader to gather information from console.
///</summary>
void SimulationDeployer::GatherInput() {
    traces.emplace_back(Trace(0,0xC84));
    traces.emplace_back(Trace(0,0x81C));
    traces.emplace_back(Trace(0,0x14C));
    traces.emplace_back(Trace(0,0xC84));
    traces.emplace_back(Trace(0,0x400));
    traces.emplace_back(Trace(0,0x148));
    traces.emplace_back(Trace(0,0x144));
    traces.emplace_back(Trace(0,0xC80));
    traces.emplace_back(Trace(0,0x008));
}


/// <summary>
/// Run our simulation. Start feeding in traces to the MemoryController.
/// Feed output to the OutputDisplayer.
/// </summary>
void SimulationDeployer::RunProgram() {
    ///DAVID: uncomment this next section. It should work. If it doesn't fix it.
    /// This next section should 
    //For each address in inputReader.inputLines
    // pass into MC, storing results in array
    //*
    while(true) {
        std::pair<bool, std::pair<std::string, std::string>> fileInput = inputReader.ReadTrace();
        if(fileInput.first == true) break;                              // if no more to read, finished.
        int accessType = 0;
        if(fileInput.second.first == "W") accessType = 1;
        int hexaddr = std::stoi(fileInput.second.second, nullptr, 16);  // get hex address
        Trace trace(accessType,hexaddr);                                // create trace
        TraceStats traceStats = MC.RunMemory(trace);                    // run
        outputDisplayer.DisplayTraceLine(traceStats);                   // display
    }
    /**/
    /* OLD
    for(int i = 0; i < traces.size(); i++) {
        outputDisplayer.DisplayTraceLine(MC.RunMemory(traces[i]));
    }
    /**/

    // Get stats from components
    HardwareStats TLBstats = MC.GetDTLBStats();
    HardwareStats PTstats = MC.GetPTStats();
    HardwareStats DCstats = MC.GetDCStats();

    outputDisplayer.FeedPTStats(PTstats);
    outputDisplayer.FeedTLBStats(TLBstats);
    outputDisplayer.FeedDCStats(DCstats);
    outputDisplayer.DisplayComponentStats();

    ReferenceStats refStats = MC.GetReferenceCounts();
    outputDisplayer.FeedReferenceOutput(refStats);
    outputDisplayer.DisplayReferenceStats();

}
