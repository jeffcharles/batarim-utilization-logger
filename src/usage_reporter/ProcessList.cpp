#include <map>

#include "ProcessList.hpp"

using std::map;
using std::pair;

namespace {
    bool process_is_active(
        map<unsigned int, ProcessInformation>::const_iterator iter
    ) {
        return iter->second.after_time > 0;
    }
}

unsigned int ProcessList::get_pid_with_highest_cpu_usage() const
{
    pair<unsigned int, unsigned long long> highest_cpu_usage;

    typedef map<unsigned int, ProcessInformation>::const_iterator Iterator;

    // Initialize highest_cpu_usage to first active process
    // NOTE: There will always be at least one active process running
    // (i.e., this one)
    Iterator iter;
    for(iter = processes_.begin(); iter != processes_.end(); ++iter) {
        if(process_is_active(iter)) {
            highest_cpu_usage = 
                pair<unsigned int, unsigned long long>(
                    iter->first,
                    iter->second.after_time - iter->second.before_time
                );
            ++iter;
            break;
        }
    }
    
    // Scan through rest of processes to see if one is higher
    for(; iter != processes_.end(); ++iter) {
        unsigned long long before_time = iter->second.before_time;
        unsigned long long after_time = iter->second.after_time;
        if(process_is_active(iter)) {
            unsigned long long usage = after_time - before_time;
            if(usage > highest_cpu_usage.second) {
                highest_cpu_usage.first = iter->first;
                highest_cpu_usage.second = usage;
            }
        }
    }

    return highest_cpu_usage.first;
}

unsigned int ProcessList::get_pid_with_highest_ram_usage() const
{
    pair<unsigned int, unsigned long long> highest_ram_usage;

    typedef map<unsigned int, ProcessInformation>::const_iterator Iterator;

    // Initialize highest_ram_usage to first active process
    // NOTE: There will always be at least one active process running
    // (i.e., this one)
    Iterator iter;
    for(iter = processes_.begin(); iter != processes_.end(); ++iter) {
        if(process_is_active(iter)) {
            highest_ram_usage = 
                pair<unsigned int, unsigned long long>(
                    iter->first,
                    iter->second.ram_usage
                );
            ++iter;
            break;
        }
    }
    
    // Scan through rest of processes to see if one is higher
    for(; iter != processes_.end(); ++iter) {
        if(process_is_active(iter)) {
            if(iter->second.ram_usage > highest_ram_usage.second) {
                highest_ram_usage.first = iter->first;
                highest_ram_usage.second = iter->second.ram_usage;
            }
        }
    }

    return highest_ram_usage.first;
}