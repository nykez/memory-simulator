//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Project:		GroupProject
//	File Name:		SimulationDeployer.h
//	Description:    Runs the simulation.
//	Course:			CSCI-4727-940: Operating Systems
//	Author:			Harrison Pollitte, pollitteh@etsu.edu, Department of Computing, East Tennessee State University
//	Created:		Monday, December 7 2020
//	Copyright:	    Harrison Pollitte 2020
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
/// <summary>
/// runs simulation
/// </summary>
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
    void Initialize(const string& configFile, const string& traceFile);
    void GatherInput();
    void RunProgram();
};


#endif //SIM_DEP_H