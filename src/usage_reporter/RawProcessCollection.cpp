#include <map>
#include <memory>
#include <vector>

#include "RawProcessCollection.hpp"

using std::map;
using std::pair;
using std::shared_ptr;
using std::vector;

void RawProcessCollection::update()
{
    shared_ptr<vector<unsigned int>> pids = get_pids_();
    for(vector<unsigned int>::const_iterator pid = pids->begin();
        pid != pids->end(); ++pid) {

        processes_[*pid].after_time = get_process_time_(*pid);
        processes_[*pid].ram_usage = get_process_ram_usage_(*pid);
    }

    // remove pids that are no longer active
    // cannot delete pid entries while iterating through the map they are
    // defined in
    typedef map<unsigned int, ProcessInformation>::iterator Iter;
    vector<unsigned int> pids_to_remove;
    for(Iter pid = processes_.begin(); pid != processes_.end(); ++pid) {
        bool is_process_valid = pid->second.after_time > 0;
        if(!is_process_valid) {
            pids_to_remove.push_back(pid->first);
        }
    }
    for(vector<unsigned int>::const_iterator pid = pids_to_remove.begin();
        pid != pids_to_remove.end(); ++pid) {

        processes_.erase(*pid);
    }
}

unsigned int RawProcessCollection::get_pid_with_highest_cpu_usage() const
{
    pair<unsigned int, unsigned long long> highest_cpu_usage;

    typedef map<unsigned int, ProcessInformation>::const_iterator Iter;

    // Initialize highest_cpu_usage to first active process
    // NOTE: There will always be at least one active process running
    // (i.e., this one)
    Iter first_proc = processes_.begin();
    highest_cpu_usage = 
        pair<unsigned int, unsigned long long>(
            first_proc->first,
            first_proc->second.after_time - first_proc->second.before_time
        );
    
    // Scan through rest of processes to see if one is higher
    for(Iter iter = processes_.begin(); iter != processes_.end(); ++iter) {
        unsigned long long before_time = iter->second.before_time;
        unsigned long long after_time = iter->second.after_time;
        unsigned long long usage = after_time - before_time;
        if(usage > highest_cpu_usage.second) {
            highest_cpu_usage.first = iter->first;
            highest_cpu_usage.second = usage;
        }
    }

    return highest_cpu_usage.first;
}

unsigned int RawProcessCollection::get_pid_with_highest_ram_usage() const
{
    pair<unsigned int, unsigned long long> highest_ram_usage;

    typedef map<unsigned int, ProcessInformation>::const_iterator Iter;

    // Initialize highest_ram_usage to first active process
    // NOTE: There will always be at least one active process running
    // (i.e., this one)
    Iter first_proc = processes_.begin();
    highest_ram_usage = 
        pair<unsigned int, unsigned long long>(
            first_proc->first,
            first_proc->second.ram_usage
        );
    
    // Scan through rest of processes to see if one is higher
    for(Iter iter = processes_.begin(); iter != processes_.end(); ++iter) {
        if(iter->second.ram_usage > highest_ram_usage.second) {
            highest_ram_usage.first = iter->first;
            highest_ram_usage.second = iter->second.ram_usage;
        }
    }

    return highest_ram_usage.first;
}