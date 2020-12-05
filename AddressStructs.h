//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Project:		GroupProject
//	File Name:		AddressStructs.h
//	Description:    Address structs
//	Course:			CSCI-4727-940: Operating Systems
//	Author:			Harrison Pollitte, pollitteh@etsu.edu, Department of Computing, East Tennessee State University
//	Created:		Wednesday, December 2 2020
//	Copyright:	    Harrison Pollitte 2020
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <vector>


/// A Logical/Virtual Address
struct VirtualAddress
{
    public:
   std::vector<int> address;    // whole bit address
   std::vector<int> VPN;        // Virtual Page Number
   std::vector<int> offset;     // offset into page

    VirtualAddress(std::vector<int> addr, int offsetSize)
    {
        // Populate address
        for(int i = 0; i < addr.size(); i++) 
        {
            address.push_back(addr.at(i));
        }
        // Populate VPN
        for(int i = 0; i < addr.size() - offsetSize; i++)
        {
            VPN.push_back(addr.at(i));
        }        
        // Populate offset
        for(int i = VPN.size(); i < addr.size(); i++) 
        {
            offset.push_back(addr.at(i));
        }
    }

};

/// A Physical Address
struct PhysicalAddress
{
    std::vector<int> address;   // whole bit address
    std::vector<int> PFN;       // Physical Frame Number
    std::vector<int> offset;    // offset into frame

    PhysicalAddress(std::vector<int> addr, int offsetSize)
    {
        // Populate address
        for(int i = 0; i < addr.size(); i++) 
        {
            address.push_back(addr.at(i));
        }
        // Populate VPN
        for(int i = 0; i < addr.size() - offsetSize; i++)
        {
            PFN.push_back(addr.at(i));
        }        
        // Populate offset
        for(int i = PFN.size(); i < addr.size(); i++) 
        {
            offset.push_back(addr.at(i));
        }
    }
};


