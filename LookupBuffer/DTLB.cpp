#include "DTLB.h"

/// <summary>
/// Default Constructor
/// </summary>
TLB::TLB()
= default;

/// <summary>
/// Parameterized Constructor
/// </summary>
/// <param name="max_table_size">The maximum permitted size for the TLB.</param>
TLB::TLB(int max_table_size)
{
	max_size_ = max_table_size;
}

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
	if (mappings_.count(VPN) > 0)
	{
		//Check if the entry's valid bit is set
		if (mappings_.at(VPN).validBit == true)
		{
			valid = mappings_.at(VPN).validBit;
			pfn = mappings_.at(VPN).PFN;
			
			//if entry is in TLB and is valid
			this->tlb_hits_++; //count TLB hit
			
			//update entry lru
			mappings_.at(VPN).lastUsed = access_ordinal_;
			access_ordinal_++;
			
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

/// <summary>
/// Inserts a specified entry at a specified VPN into the TLB. If the TLB's
/// max size is reached, the least recently used entry is evicted.
/// </summary>
/// <param name="VPN">The VPN to insert.</param>
/// <param name="entry">The entry to insert.</param>
void TLB::InsertEntry(int VPN, TableEntry entry)
{
	auto vpn_to_insert = VPN; //get VPN to insert
	auto entry_to_insert = entry; //get entry to insert
	entry_to_insert.lastUsed = GetAccessOrdinal();
	access_ordinal_++;

	//Check if the TLB has free space
	if (mappings_.size() < max_size_)
	{
		//If there is free space, add entry to TLB
		mappings_.insert({vpn_to_insert, entry_to_insert});
		
	}
	else
	{
		//If there isn't free space, evict lru entry
		auto lru = this->GetAccessOrdinal();
		auto victim_key = -1;

		for (const auto mapping : mappings_)
		{
			//update lru
			if (mapping.second.lastUsed < lru)
			{
				lru = mapping.second.lastUsed; //set new LRU
				victim_key = mapping.first; //set new Key/VPN
			}
		}

		//Erase lru entry
		mappings_.erase(victim_key);
		mappings_.insert({vpn_to_insert, entry_to_insert});
	}
}

/// <summary>
/// Gets the ordinal access of the TLB.
/// </summary>
/// <returns></returns>
int TLB::GetAccessOrdinal()
{
	return this->access_ordinal_;
}

/// <summary>
/// Get the TLB entry's valid bit.
/// </summary>
/// <param name="VPN">The VPN to access.</param>
/// <returns>The VPN's corresponding valid bit state.</returns>
bool TLB::GetEntryValidity(int VPN)
{
	if (mappings_.count(VPN) > 0)
	{
		return mappings_.at(VPN).validBit;
	}
	else
	{
		//std::cerr << "Couldn't get entry validity: Entry '" << VPN << "' not in TLB" << std::endl;
		return false;
	}
}

/// <summary>
/// Sets a specified entry's valid bit.
/// </summary>
/// <param name="VPN">The VPN to access.</param>
/// <param name="state">The state of the valid bit for the corresponding VPN</param>
void TLB::SetEntryValidity(int VPN, bool state)
{
	if (mappings_.count(VPN) > 0)
	{
		mappings_.at(VPN).validBit = state;
	}
	else
	{
		//std::cerr << "Couldn't set entry validity: Entry '" << VPN << "' not in TLB" << std::endl;
	}
}

/// <summary>
/// Gets the dirty bit of the specified TLB entry.
/// </summary>
/// <param name="VPN">The VPN of the TLB entry.</param>
/// <returns>The dirty bit of the TLB entry.</returns>
bool TLB::GetEntryDirty(int VPN)
{
	if (mappings_.count(VPN) > 0)
	{
		return mappings_.at(VPN).dirtyBit;
	}
	else
	{
		//std::cerr << "Couldn't get entry dirty bit: Entry '" << VPN << "' not in TLB" << std::endl;
		return false;
	}
}

/// <summary>
/// Sets the dirty bit for the specified table entry.
/// </summary>
/// <param name="VPN">The VPN for the table entry.</param>
/// <param name="state">The state of the dirty bit for the corresponding VPN.</param>
void TLB::SetEntryDirty(int VPN, bool state)
{
	if (mappings_.count(VPN) > 0)
	{
		mappings_.at(VPN).dirtyBit = state;
	}
	else
	{
		//std::cerr << "Couldn't set entry dirty bit: Entry '" << VPN << "' not in TLB" << std::endl;
	}
}

/// <summary>
/// Sets the dirty bit for the specified table entry.
/// </summary>
/// <returns>Returns the hits and misses from this TLB.</returns>
HardwareStats TLB::GetStatistics() {
	HardwareStats stats(tlb_hits_, tlb_misses_);
	return stats;
}

/// <summary>
/// Gets the LRU for the entry corresponding to the specified VPN.
/// </summary>
/// <param name="VPN">The TLB entry's VPN.</param>
/// <returns>The LRU for the TLB entry.</returns>
int TLB::GetEntryAccessOrdinal(int VPN)
{
	if (mappings_.count(VPN) > 0)
	{
		return mappings_.at(VPN).lastUsed;
	}
	else
	{
		//std::cerr << "Couldn't get entry LRU: Entry '" << VPN << "' not in TLB" << std::endl;
		return -1;
	}
}

/// <summary>
/// Gets the maximum allowed size for the TLB.
/// </summary>
/// <returns>The TLB's max size.</returns>
int TLB::GetMaxSize()
{
	return this->max_size_;
}

/// <summary>
/// Sets the maximum allowed size of the TLB.
/// </summary>
/// <param name="max_size">The maximum size of the TLB.</param>
void TLB::SetMaxSize(int max_size)
{
	this->max_size_ = max_size;
}

/// <summary>
/// Gets the current size of the TLB.
/// </summary>
/// <returns>The TLB's current size.</returns>
int TLB::GetSize()
{
	return this->mappings_.size();
}
