/// NAME: SimulationDeployer.h
/// PURPOSE: Main controller of the program. Initial class.
/// AUTHOR: Harrison Lee Pollitte
/// CREATED: 12/6/2020
/// NOTES:
/// * Where we start program (after main)
/// * Calls InputHandler to get input, then calls MemoryController to run simulation
/// * Then calls OutputDisplayer to display output. 
/// * It also does math for counting number of bits and shit and stuff

#ifndef SIM_DEP_H
#define SIM_DEP_H

#include "MemoryController.cpp"
#include "MemoryOptions.h"
#include "InputReader.h"
#include "OutputDisplayer.cpp"
#include "Trace.h"
#include "TraceStats.h"
#include <string>
#include <vector>

class SimulationDeployer {
private:
    InputReader inputReader;
    OutputDisplayer outputDisplayer;
    MemoryController MC;        //ptr to MemoryController of this program simulation
    vector<Trace> traces;
    MemoryOptions MO;
public:
    SimulationDeployer();
    ~SimulationDeployer();

    /// <summary>
    /// Read config file and set up Memory Controller
    /// </summary>
    void Initialize();


    void GatherInput();


    void RunProgram();

};

SimulationDeployer::SimulationDeployer() {
}

SimulationDeployer::~SimulationDeployer() {
}

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
    outputDisplayer.FeedMiscOutput(refStats);

    MemoryOptions Options = MC.GetConfigOptions();
    outputDisplayer.FeedConfigOutput(Options);
}



#endif //SIM_DEP_H