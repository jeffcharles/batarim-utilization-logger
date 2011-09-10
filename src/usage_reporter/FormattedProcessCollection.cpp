#include <map>
#include <memory>
#include <utility>

#include "ProcessUsageInfo.hpp"

#include "FormattedProcessCollection.hpp"

using std::map;
using std::pair;
using std::shared_ptr;

const ProcessUsageInfo&
FormattedProcessCollection::get_process_with_highest_cpu_usage() const
{
    pair<unsigned int, unsigned long long> highest_cpu_usage;

    typedef map<unsigned int, ProcessUsageInfo>::const_iterator Iter;

    // Initialize highest_cpu_usage to first active process
    // NOTE: There will always be at least one active process running
    // (i.e., this one)
    Iter first_proc = usage_info_.begin();
    highest_cpu_usage = 
        pair<unsigned int, unsigned long long>(
            first_proc->first,
            first_proc->second.cpu_usage
        );
    
    // Scan through rest of processes to see if one is higher
    for(Iter iter = usage_info_.begin(); iter != usage_info_.end(); ++iter) {
        if(iter->second.cpu_usage > highest_cpu_usage.second) {
            highest_cpu_usage.first = iter->first;
            highest_cpu_usage.second = iter->second.cpu_usage;
        }
    }

    return usage_info_.at(highest_cpu_usage.first);
}

const ProcessUsageInfo&
FormattedProcessCollection::get_process_with_highest_ram_usage() const
{
    pair<unsigned int, unsigned long long> highest_ram_usage;

    typedef map<unsigned int, ProcessUsageInfo>::const_iterator Iter;

    // Initialize highest_ram_usage to first active process
    // NOTE: There will always be at least one active process running
    // (i.e., this one)
    Iter first_proc = usage_info_.begin();
    highest_ram_usage = 
        pair<unsigned int, unsigned long long>(
            first_proc->first,
            first_proc->second.ram_usage
        );
    
    // Scan through rest of processes to see if one is higher
    for(Iter iter = usage_info_.begin(); iter != usage_info_.end(); ++iter) {
        if(iter->second.ram_usage > highest_ram_usage.second) {
            highest_ram_usage.first = iter->first;
            highest_ram_usage.second = iter->second.ram_usage;
        }
    }

    return usage_info_.at(highest_ram_usage.first);
}