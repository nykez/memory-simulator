

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