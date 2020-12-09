#pragma once

#ifndef TLB_H
#define TLB_H

#include <iostream>
#include <map>

#include "../TableEntry.h"
#include "../HardwareStats.h"

/// <summary>
/// The Data Translation Look-aside Buffer for the memory simulator.
/// </summary>
class TLB
{
	//TLB statistics
	int tlb_references_ = 0; //amount of times DTLB has been referenced
	int tlb_hits_ = 0; //TLB hit counter
	int tlb_misses_ = 0; //TLB miss counter

	//TLB properties
	int access_ordinal_ = 1; //largest == most recently used
	int max_size_ = 0; //maximum size of DTLB
	
	std::map<int, TableEntry> mappings_; //map to hold TLB mappings <VPN:TableEntry>
	
public:

	TLB();
	TLB(int max_table_size);

	std::pair<bool, int> LookUp(int VPN);
	void InsertEntry(int VPN, TableEntry entry);
	
	int GetAccessOrdinal();
	bool GetEntryValidity(int VPN);
	void SetEntryValidity(int VPN, bool state);
	bool GetEntryDirty(int VPN);
	void SetEntryDirty(int VPN, bool state);
	int GetEntryAccessOrdinal(int VPN);
	int GetMaxSize();
	void SetMaxSize(int max_size);
	int GetSize();
	HardwareStats GetStatistics();
};


#endif // TLB_H
