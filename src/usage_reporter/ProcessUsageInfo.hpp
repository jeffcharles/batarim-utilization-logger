#ifndef GUARD_ProcessUsageInfo_h
#define GUARD_ProcessUsageInfo_h

#include <string>

struct ProcessUsageInfo
{
    std::string process_name;
    int cpu_usage;
    int ram_usage;
};

#endif