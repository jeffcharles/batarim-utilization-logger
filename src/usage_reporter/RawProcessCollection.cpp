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