#ifndef TLB_H
#define TLB_H
#include <map>

#include "DTLBEntry.h"
#include "../BinaryConverter.h"

class TLB
{
	//TLB statistics
	int tlb_hits_ = 0; //TLB hit counter
	int tlb_misses_ = 0; //TLB miss counter

	std::map<int, DTLBEntry> mappings; //map to hold TLB mappings <VPN:TableEntry>
	
public:

	TLB();

	std::pair<bool, int> LookUp(int VPN);
};

TLB::TLB()
= default;

/// <summary>
/// Queries the TLB for a specified VPN and returns a pair that tells if the
/// entry was valid and the associated PFN. Pair is <false:-1> if entry
/// couldn't be found.
/// </summary>
/// <param name="VPN">The VPN to look up in the TLB.</param>
/// <returns>A pair specifying if the entry was valid and the corresponding PFN
/// of the entry.</returns>
std::pair<bool, int> TLB::LookUp(int VPN)
{
	auto valid = false; //initialize the found entry's valid bit to false
	auto pfn = -1; //initialize the found entry's PFN to -1

	for (auto mapping : mappings)
	{
		valid = mapping.second.validBit; //is the entry valid?

		//If the queried VPN is found and is valid
		if (VPN == mapping.first && valid == true)
		{
			//Return the corresponding PFN
			pfn = mapping.second.PFN;

			this->tlb_hits_++; //increment the number of TLB hits
			return std::pair<bool, int>{valid, pfn}; //return valid PFN
		}
	}

	//if the pfn couldn't be found
	return std::pair<bool, int>{valid, pfn}; //return pair indicating pfn not found
}
#endif // TLB_H
