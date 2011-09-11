#include <map>
#include <string>
#include <utility>

#include "../formatted_process_collection/ProcessUsageInfo.hpp"

#include "AggregatedProcessCollection.hpp"

using std::map;
using std::pair;
using std::string;

void AggregatedProcessCollection::update(
    const FormattedProcessCollection& processes
) {
    typedef map<unsigned int, ProcessUsageInfo>::const_iterator Iter;
    for(Iter iter = processes.begin(); iter != processes.end(); ++iter) {

        const ProcessUsageInfo& proc_info = iter->second;
        const std::string& proc_name = proc_info.process_name;
        
        bool need_to_initialize_info =
            aggregated_processes_.find(proc_name) == 
            aggregated_processes_.end();
        if(need_to_initialize_info) {
            ProcessUsageInfo initial_info;
            initial_info.process_name = proc_name;
            initial_info.cpu_usage = 0;
            initial_info.ram_usage = 0;
            aggregated_processes_[proc_name] = initial_info;
        }
        
        ProcessUsageInfo& aggregate_info = aggregated_processes_[proc_name];
        aggregate_info.cpu_usage += proc_info.cpu_usage;
        aggregate_info.ram_usage += proc_info.ram_usage;
        
        aggregated_processes_[proc_name] = aggregate_info;
    }
}

const ProcessUsageInfo&
AggregatedProcessCollection::get_aggregate_with_highest_cpu_usage() const
{
    pair<string, int> highest_cpu_usage;

    typedef map<string, ProcessUsageInfo>::const_iterator Iter;

    // Initialize highest_cpu_usage to first active process
    // NOTE: There will always be at least one active process running
    // (i.e., this one)
    Iter first_proc = aggregated_processes_.begin();
    highest_cpu_usage = 
        pair<string, int>(
            first_proc->first,
            first_proc->second.cpu_usage
        );
    
    // Scan through rest of processes to see if one is higher
    for(Iter iter = aggregated_processes_.begin();
        iter != aggregated_processes_.end(); ++iter) {
        
        if(iter->second.cpu_usage > highest_cpu_usage.second) {
            highest_cpu_usage.first = iter->first;
            highest_cpu_usage.second = iter->second.cpu_usage;
        }
    }

    return aggregated_processes_.at(highest_cpu_usage.first);
}

const ProcessUsageInfo&
AggregatedProcessCollection::get_aggregate_with_highest_ram_usage() const
{
    pair<string, int> highest_ram_usage;

    typedef map<string, ProcessUsageInfo>::const_iterator Iter;

    // Initialize highest_ram_usage to first active process
    // NOTE: There will always be at least one active process running
    // (i.e., this one)
    Iter first_proc = aggregated_processes_.begin();
    highest_ram_usage = 
        pair<string, int>(
            first_proc->first,
            first_proc->second.ram_usage
        );
    
    // Scan through rest of processes to see if one is higher
    for(Iter iter = aggregated_processes_.begin();
        iter != aggregated_processes_.end(); ++iter) {
        
        if(iter->second.ram_usage > highest_ram_usage.second) {
            highest_ram_usage.first = iter->first;
            highest_ram_usage.second = iter->second.ram_usage;
        }
    }

    return aggregated_processes_.at(highest_ram_usage.first);
}