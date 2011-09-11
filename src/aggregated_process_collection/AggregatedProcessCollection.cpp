#include <map>
#include <string>

#include "../formatted_process_collection/ProcessUsageInfo.hpp"

#include "AggregatedProcessCollection.hpp"

using std::map;
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