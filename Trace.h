//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Project:		GroupProject
//	File Name:		Trace.h
//	Description:    A trace! Access type and address.
//	Course:			CSCI-4727-940: Operating Systems
//	Author:			Harrison Pollitte, pollitteh@etsu.edu, Department of Computing, East Tennessee State University
//	Created:		Sunday, December 6 2020
//	Copyright:	    Harrison Pollitte 2020
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef TRACE_H
#define TRACE_H
#include <string>

/// <summary>
/// 0: read 1: write
/// </summary>
enum class AccessType {
    Read, Write
};


///<summary>
/// For representing address traces
///</summary>
struct Trace
{
public:
    AccessType accessType;  // 0: read 1: write
    int hexAddress;         // address

    /// <summary>
    /// Default
    /// </summary>
    Trace() {
        accessType = AccessType::Read;
        hexAddress = 0;       
    }

    /// <summary>
    /// Parameterized constructor.
    /// </summary>
    /// <param name="access">access type</param>
    /// <param name="addr">address</param> 
    Trace(int access, int addr) {
        accessType = (AccessType)access;
        hexAddress = addr;
    }
};

#endif