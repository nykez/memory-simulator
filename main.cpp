#include "SimulationDeployer.cpp"
#include "TraceStats.h"
#include "Trace.h"
// main entry point for program
int main()
{
    Trace trace(0,50);
    TraceStats trs(trace);

    SimulationDeployer SD;
    SD.Initialize("trace.config", "trace.dat");
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