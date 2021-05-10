
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