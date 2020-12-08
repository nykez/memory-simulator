#ifndef TLB_H
#define TLB_H

#include <iostream>
#include <map>

#include "../TableEntry.h"

class TLB
{
	//TLB statistics
	int tlb_references_ = 0; //amount of times DTLB has been referenced
	int tlb_hits_ = 0; //TLB hit counter
	int tlb_misses_ = 0; //TLB miss counter

	//TLB properties
	int accessOrdinal = 1; //largest == most recently used
	int maxSize = 0; //maximum size of DTLB
	int size = 0; //current size of TLB
	
	std::map<int, TableEntry> mappings; //map to hold TLB mappings <VPN:TableEntry>
	
public:

	TLB();

	std::pair<bool, int> LookUp(int VPN);
	void InsertEntry(TableEntry entry);
	
	int GetAccessOrdinal();
	bool GetEntryValidity(int entry);
	void SetEntryValidity(int entry, bool state);
	int GetMaxSize();
	void SetMaxSize(int maxSize);
	int GetSize();
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
	this->tlb_references_++; //update TLB references counter
	
	auto valid = false; //initialize the found entry's valid bit to false
	auto pfn = -1; //initialize the found entry's PFN to -1
	
	//If the TLB contains the queried VPN
	if (mappings.count(VPN) > 0)
	{
		//Check if the entry's valid bit is set
		if (mappings.at(VPN).validBit == true)
		{
			valid = mappings.at(VPN).validBit;
			pfn = mappings.at(VPN).PFN;
			
			//if entry is in TLB and is valid
			this->tlb_hits_++; //count TLB hit
			
			//update entry lru
			mappings.at(VPN).lastUsed = accessOrdinal;
			accessOrdinal++;
			
			return {valid, pfn}; //return pair indicating entry is valid
		}
		else
		{
			//If the entry is invalid
			this->tlb_misses_++; //count TLB miss
			return {valid, pfn}; //return pair indicating entry is invalid
		}
	}
	else
	{
		//If the pfn couldn't be found
		this->tlb_misses_++; //count TLB miss
		return {valid, pfn}; //return pair indicating pfn not found
	}
}

inline void TLB::InsertEntry(TableEntry entry)
{
	//Check if the TLB has free space
	if (mappings.size() < maxSize)
	{
		//If there is free space, add entry to TLB
		mappings.insert({entry.validBit, entry});
	}
	else
	{
		//If there isn't free space, evict entry
		
	}
}

int TLB::GetAccessOrdinal()
{
	return this->accessOrdinal;
}

bool TLB::GetEntryValidity(int VPN)
{
	if (mappings.count(VPN) == true)
	{
		return mappings.at(VPN).validBit;
	}
	else
	{
		std::cerr << "Entry '" << VPN << "' not in table" << std::endl;
		return false;
	}
}

void TLB::SetEntryValidity(int VPN, bool state)
{
	
}

int TLB::GetMaxSize()
{
	return this->maxSize;
}

void TLB::SetMaxSize(int maxSize)
{
	this->maxSize = maxSize;
}

int TLB::GetSize()
{
	return this->size;
}
#endif // TLB_H
