#ifndef MEM_CON_H
#define MEM_CON_H
#include "DataCache/Cache.h"
#include "PageTable.h"
#include "BinaryConverter.h"
#include "PageFaultHandler.h"

class MemoryController {
private:
    //DTLB TLB;         // our TLB
    Cache::Cache DC;    // our data cache
    PageTable PT;       // our page table
public:
    MemoryController(int set, int index, int pages, int frames, int pageSize);
    ~MemoryController();
    int TransformVPNToPFN(int VPN);
    int HandleFault(int VPN);
    void CreateElements();
};

MemoryController::MemoryController(int set, int index, int pages, int frames, int pageSize) {
    DC = Cache(set,index);
    PT = PageTable(pages, frames, pageSize);
}

MemoryController::~MemoryController() {
}

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