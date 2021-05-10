
#ifndef TRACE_STATS_H
#define TRACE_STATS_H

#include <string>
#include "Trace.h"

///<summary>
/// For tracking statistics of a trace.
///</summary>
struct TraceStats
{
    Trace trace;            // original trace (<accessType>:<hexaddress>)
    int VPN                 = 0;    // HEX
    int pageOffset          = 0;    // HEX
    std::string TLBresult   = "  "; // HIT/MISS/NA
    std::string PTresult    = "  "; // HIT/MISS/NA
    int PFN                 = 0;    // HEX
    int DCtag               = 0;    // HEX
    int DCidx               = 0;    // HEX
    std::string DCresult    = "MISS";    // HIT/MISS

    /// <summary>
    /// Parameterized constructor.
    /// </summary>
    /// <param name="tr">the initial trace.</param>
    TraceStats(Trace tr) {
        trace = tr;
    }
};
#endif