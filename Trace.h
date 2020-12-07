#ifndef TRACE_H
#define TRACE_H
#include <string>

enum class AccessType {
    Read, Write
};


///<summary>
/// For representing address traces
///</summary>
struct Trace
{
public:
    AccessType accessType;
    int hexAddress;

    Trace() {
        accessType = AccessType::Read;
        hexAddress = 0;       //NOTE: A string with 9 chars has 10 chars (NULL)
    }

    Trace(int access, int addr) {
        accessType = (AccessType)access;
        hexAddress = addr;
    }
};

#endif