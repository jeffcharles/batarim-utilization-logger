#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../formatted_process_collection/FormattedProcessCollection.hpp"

#include "ProcessTree.hpp"

using std::map;
using std::unordered_map;
using std::unordered_set;
using std::vector;

void ProcessTree::update(const FormattedProcessCollection& processes)
{
    unordered_map<unsigned int, unsigned int> lookup_table;

    for(FormattedProcessCollection::const_iterator iter = processes.begin();
        iter != processes.end(); ++iter) {

        unsigned int top_level_pid =
            get_top_level_pid_(iter->first, processes, lookup_table);

        bool process_initialized =
            top_level_processes_.find(top_level_pid) !=
            top_level_processes_.end();
        if(!process_initialized) {
            ProcessUsageInfo initial_info;
            initial_info.cpu_usage = 0;
            initial_info.process_name =
                processes.get_process(top_level_pid).process_name;
            initial_info.ram_usage = 0;
            top_level_processes_[top_level_pid] = initial_info;
        }

        ProcessUsageInfo& info = top_level_processes_[top_level_pid];
        info.cpu_usage += iter->second.cpu_usage;
        info.ram_usage += iter->second.ram_usage;
    }
}

const unsigned int ProcessTree::get_top_level_pid_(
    const unsigned int pid,
    const FormattedProcessCollection& processes,
    unordered_map<unsigned int, unsigned int>& lookup_table
) const {

    if(process_is_top_level_(pid, processes)) {
        return pid;
    }

    bool process_in_lookup_table = 
        lookup_table.find(pid) != lookup_table.end();
    if(process_in_lookup_table) {
        return lookup_table.at(pid);
    }
    
    return climb_tree_and_build_lookup_table_(pid, processes, lookup_table);
}

// Climbs the tree and builds the lookup table after climbing the tree.
// The top level PID is returned.
const unsigned int ProcessTree::climb_tree_and_build_lookup_table_(
    const unsigned int pid,
    const FormattedProcessCollection& processes,
    unordered_map<unsigned int, unsigned int>& lookup_table
) const {
    
    // build list of intermediate PIDs as the tree is climbed
    unordered_set<unsigned int> intermediate_pids;
    unsigned int current_pid = processes.get_ppid(pid);
    while(!process_is_top_level_(current_pid, processes)) {
        bool pid_in_lookup =
            lookup_table.find(current_pid) != lookup_table.end();
        if(pid_in_lookup) {
            current_pid = lookup_table.at(current_pid);
            break;
        }
        
        // cyclic relationships can occur with the current Windows
        // implementation of finding parent process
        bool cyclic_relationship_present = 
            intermediate_pids.count(current_pid) > 0;
        if(cyclic_relationship_present) {
            break;
        }
        intermediate_pids.insert(current_pid);

        current_pid = processes.get_ppid(current_pid);
    }
    unsigned int top_level_pid = current_pid;
    
    // assign entries in lookup table for this PID and all encountered
    // intermediate ones
    lookup_table[pid] = top_level_pid;
                
    typedef unordered_set<unsigned int>::const_iterator Iter;
    for(Iter iter = intermediate_pids.begin();
        iter != intermediate_pids.end(); ++iter) {

        lookup_table[*iter] = top_level_pid;
    }

    return top_level_pid;
}
