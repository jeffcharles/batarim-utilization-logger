#include <map>
#include <memory>
#include <vector>

#include "../utilities/utilities.hpp"

#include "RawProcessCollection.hpp"

using std::map;
using std::pair;
using std::unique_ptr;
using std::vector;

void RawProcessCollection::update()
{
    platform_specific_update_();

    // remove pids that are no longer active
    // cannot delete pid entries while iterating through the map they are
    // defined in
    typedef map<unsigned int, ProcessInformation>::iterator Iter;
    vector<unsigned int> pids_to_remove;
    for(Iter pid = processes_.begin(); pid != processes_.end(); ++pid) {
        bool is_process_valid =
            pid->second.last_update_run_ == current_update_run_;
        if(!is_process_valid) {
            pids_to_remove.push_back(pid->first);
        }
    }
    for(vector<unsigned int>::const_iterator pid = pids_to_remove.begin();
        pid != pids_to_remove.end(); ++pid) {

        processes_.erase(*pid);
    }
    ++current_update_run_;
}