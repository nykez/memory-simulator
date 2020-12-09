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
}