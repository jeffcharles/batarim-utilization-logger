#include <string>
#include <vector>

#include "../formatted_process_collection/FormattedProcessCollection.hpp"

#include "WindowsProcessTree.hpp"

using std::string;
using std::vector;

bool WindowsProcessTree::process_is_top_level_(
    unsigned int pid,
    const FormattedProcessCollection& processes
) const {
    const unsigned int ppid = processes.get_ppid(pid);
    if(ppid == 0) {
        return true;
    }

    if(!processes.contains_pid(ppid)) {
        return true;
    }

    const string parent_process_name =
        processes.get_process(ppid).process_name;
    vector<string> top_level_processes;
    top_level_processes.push_back("services");
    top_level_processes.push_back("explorer");
    for(vector<string>::const_iterator iter = top_level_processes.begin();
        iter != top_level_processes.end(); ++iter) {
                
        if(parent_process_name == *iter) {
            return true;
        }
    }

    return false;
}