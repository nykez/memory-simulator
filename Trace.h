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
    std::string hexAddress;

    Trace() {
        accessType = AccessType::Read;
        hexAddress = "000000000";       //NOTE: A string with 9 chars has 10 chars (NULL)
    }

    Trace(int access, std::string& addr) {
        accessType = (AccessType)access;
        hexAddress.clear();
        hexAddress = addr;
        hexAddress.shrink_to_fit();
    }
};

#endif