
#include "DataCache/Cache.h"
#include "PageTable.h"
#include "BinaryConverter.h"
#include "PageFaultHandler.h"

class MemoryController {
private:
    //DTLB TLB;           // our TLB
    //Cache DC;           // our data cache
    PageTable PT;       // our page table
public:
    MemoryController(/* args */);
    ~MemoryController();
    int TransformAddress(int VPN);
    int HandleFault(int VPN);
    void GatherInput();
    void CreateElements();
    void CreateDC();
    void CreatePT();
    void CreateTLB();
    void DisplayOutput();
};

MemoryController::MemoryController(/* args */) {

}

MemoryController::~MemoryController() {
}

int MemoryController::TransformAddress(int VPN) {
    std::vector<int> VPNarr = BinaryConverter::ToBitArray(VPN);
    int PFN = -1;
    //std::pair<bool,int> res = TLB.Translate(VPN);
    std::pair<bool,int> res = {false, 0}; //PLACEHOLDER
    if(res.first == false) {                // TLB MISS
        res = PT.TranslateVPN(VPNarr);      // Check PT
        if(res.first == false) {            // PT MISS
        PFN = HandleFault(VPN);// PageFault
        } else PFN = res.second;
    } else PFN = res.second;
    return PFN;
}

int MemoryController::HandleFault(int VPN) {
    PageFaultHandler::HandleFault(&PT, &DC, VPN);
}