/// NAME: SimulationDeployer.h
/// PURPOSE: Main controller of the program. Initial class.
/// AUTHOR: Harrison Lee Pollitte
/// CREATED: 12/6/2020
/// NOTES:
///
///
///

#ifndef SIM_DEP_H
#define SIM_DEP_H

#include "MemoryController.h"
#include <vector>

class SimulationDeployer {
private:
    //InputReader inputReader;
    //OutputDisplayer outputDisplayer;
    MemoryController* MCptr;        //ptr to MemoryController of this program simulation
    
public:
    SimulationDeployer();
    ~SimulationDeployer();
    void GatherInput();
    void RunProgram();
};

SimulationDeployer::SimulationDeployer() {

}

SimulationDeployer::~SimulationDeployer() {

}

///<summary>
/// Directs inputReader to gather information from console.
///</summary>
void SimulationDeployer::GatherInput() {
    
}

void SimulationDeployer::RunProgram() {
    int _dbg_inputreader_inputlines_count = 8;
    std::vector<string> outputLines;
    //For each address in inputReader.inputLines
    // pass into MC, storing results in array
    for(int i = 0; i < _dbg_inputreader_inputlines_count; i++) {
        outputLines[i] = MCptr->RunMemory();
    }


}



#endif //SIM_DEP_H