#ifndef DTLBENTRY_H
#define DTLBENTRY_H

struct DTLBEntry
{
public:
	//Fields
	int PFN = 0; //virtural page number of the entry
	bool dirtyBit = false; //dirty bit of the entry
	bool validBit = false; //valid bit of the entry
	int lastused = 0; //lower means less recently used; 0 means never used


	DTLBEntry(int pfn, bool dirty_bit, bool valid_bit, int lastused)
	{
		this->PFN = pfn;
		this->dirtyBit = dirty_bit;
		this->validBit = valid_bit;
		this->lastused = lastused;
	}
};
#endif // DTLBENTRY_H
