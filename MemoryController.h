#ifndef MEM_CON_H
#define MEM_CON_H
#include "DataCache/Cache.h"
#include "PageTable/PageTable.h"
#include "BinaryConverter.h"
#include "PageFaultHandler.h"
#include "Trace.h"
#include "TraceStats.h"
#include "MemoryOptions.h"
class MemoryController {
private:
    //DTLB TLB;             // our TLB
    Cache::Cache DC;        // our data cache
    PageTable PT;           // our page table

    // Configuration
    bool useVirtualMemory;  // true: use PT and TLB. False: only use DC
    bool useTLB;            // true: use PT,TLB,DC. False: use PT, DC

public:
    /// Constructor: 
    MemoryController(MemoryOptions config);
    /// die
    ~MemoryController();

    // public interface for using controller.
    TraceStats RunMemory(Trace trace);

    
    int TransformVPNToPFN(int VPN);

    /// Calls pagefault handler
    int HandleFault(int VPN);
    
};

MemoryController::MemoryController(MemoryOptions config) {
    ///TODO: TLB
    DC = Cache(config.dcEntries, config.dcSetSize);
    PT = PageTable(config.pageCount, config.frameCount, config.pageSize);

    // Configure controller
    this->useVirtualMemory = config.useVirt;
    if(this->useVirtualMemory == false) this->useTLB == false;
    else this->useTLB = config.useTLB;
}

MemoryController::~MemoryController() {
}


TraceStats MemoryController::RunMemory(Trace trace) {
    if(this->useVirtualMemory) {
        // convert trace.hexAddr to VPN 
        int VPN = 0;
    }

}

int MemoryController::UseTLB(int VPN) {
    int PFN = -1;
    std::pair<bool,int> res = {false, 0}; // PLACEHOLDER
    if(res.first == false)  {             // TLB MISS
        
    }
}
/* TLB FALSE (INPUT: Trace trace {AccessType accessType, std::string hexAddress})
std::vector<int> VPNarr = BinaryConverter::ToBitArray(VPN);
int PFN = -1;
std::pair<bool,int> res = PT.TranslateVPN(VPNarr);
if(res.first == false)
    PFN = HandleFault(VPN);
else PFN =res.second;
return PFN;
*/

/* VirtAddr FALSE
std::vector<int> 

*/
int MemoryController::TransformVPNToPFN(int VPN) {
    std::vector<int> VPNarr = BinaryConverter::ToBitArray(VPN);
    int PFN = -1;
    //std::pair<bool,int> res = TLB.Translate(VPN);
    std::pair<bool,int> res = {false, 0}; //PLACEHOLDER
    if(res.first == false) {                // TLB MISS
        res = PT.TranslateVPN(VPNarr);      // Check PT
        if(res.first == false) {            // PT MISS
        PFN = HandleFault(VPN);             // PageFault
        } else PFN = res.second;            // PT HIT
    } else PFN = res.second;                // TLB HIT
    return PFN;                             // Return PFN
}

int MemoryController::HandleFault(int VPN) {
    PageFaultHandler::HandleFault(&PT, &DC, VPN);
}
#endif