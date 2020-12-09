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
/// Default constructor
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
void SimulationDeployer::Initialize() {
    // Read config
    ///DAVID:
    // Setup config
    
    // Setup Memory Controller
    MC = (MO);
    // Display config
    outputDisplayer.FeedConfigOutput(MC.GetConfigOptions());
    outputDisplayer.DisplayConfig();
    // Setup trace file
    outputDisplayer.DisplayTraceHeader();
    ///DAVID:
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
    //For each address in inputReader.inputLines
    // pass into MC, storing results in array
    /*
    while(true) {
        std::pair<bool, std::pair<std::string, std::string>> fileInput = inputReader.ReadTraceFile();
        if(fileInput.first == true) break;                              // if no more to read, finished.
        int accessType = std::stoi(fileInput.second.first,nullptr,10);  // get access type
        int hexaddr = std::stoi(fileInput.second.second, nullptr, 16);  // get hex address
        Trace trace(accessType,hexaddr);
        TraceStats traceStats = MC.RunMemory(trace);
        outputDisplayer.DisplayTraceLine(traceStats);
    }
    /**/
    //* 
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

    ReferenceStats refStats = MC.GetReferenceCounts();
    outputDisplayer.FeedReferenceOutput(refStats);

    MemoryOptions Options = MC.GetConfigOptions();
    outputDisplayer.FeedConfigOutput(Options);
}