

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
    std::string VPN         = "0";  // HEX
    std::string pageOffset  = "0";  // HEX
    std::string TLBresult   = "NA"; // HIT/MISS/NA
    std::string PTresult    = "NA"; // HIT/MISS/NA
    std::string PFN         = "0";  // HEX
    std::string DCtag       = "0";  // HEX
    std::string DCidx       = "0";  // HEX
    std::string DCresult    = "0";  // HIT/MISS
};
#endif