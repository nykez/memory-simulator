#include "SimulationDeployer.h"
#include "TraceStats.h"
#include "Trace.h"
// main entry point for program
int main()
{
    Trace trace(0,50);
    TraceStats trs(trace);

    SimulationDeployer SD;
    SD.Initialize();
    SD.GatherInput();
    SD.RunProgram();
    // read in config
    // setup config
    // branch out config
    // read lines
    // do logic
    // print current logic
    // print end stats

}