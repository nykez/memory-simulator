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

#include "MemoryController.h"
#include "MemoryOptions.h"
#include "OutputDisplayer.h"
#include "Trace.h"
#include "TraceStats.h"

#include <vector>

class SimulationDeployer {
private:
    //InputReader inputReader;
    OutputDisplayer outputDisplayer;
    MemoryController MC;        //ptr to MemoryController of this program simulation
    vector<Trace> traces;
public:
    SimulationDeployer();
    ~SimulationDeployer();
    void GatherInput();
    void RunProgram();
};

SimulationDeployer::SimulationDeployer() {
    MemoryOptions MO;
    MC = MemoryController(MO);
}

SimulationDeployer::~SimulationDeployer() {
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
    std::vector<TraceStats> traceStats;
    //For each address in inputReader.inputLines
    // pass into MC, storing results in array
    for(int i = 0; i < traces.size(); i++) {
        traceStats.push_back(MC.RunMemory(traces[i]));
    }
    // Add to output
    for(int i = 0; i < traceStats.size(); i++) {
        outputDisplayer.AddReferenceInfo(traceStats[i]);
    }
    
    outputDisplayer.DisplayReferenceInfo();
}



#endif //SIM_DEP_H

class Mock_InputReader
{
private:
    /* data */
public:
    Mock_InputReader(/* args */);
    ~Mock_InputReader();
};

Mock_InputReader::Mock_InputReader(/* args */)
{
}

Mock_InputReader::~Mock_InputReader()
{
}
