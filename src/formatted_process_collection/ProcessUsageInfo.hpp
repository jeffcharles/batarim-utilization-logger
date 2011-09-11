#ifndef GUARD_ProcessUsageInfo_h
#define GUARD_ProcessUsageInfo_h

#include <functional>
#include <map>
#include <string>
#include <utility>

struct ProcessUsageInfo
{
    std::string process_name;
    int cpu_usage;
    int ram_usage;
};

namespace batarim {

    template<class Key> const ProcessUsageInfo& get_max_usage_in_map(
        const std::map<Key, ProcessUsageInfo>& proc_map,
        std::function<int(const ProcessUsageInfo&)> get_usage
    ) {
        std::pair<Key, int> highest_usage;

        typedef class std::map<Key, ProcessUsageInfo>::const_iterator Iter;

        // Initialize highest_usage to first active process
        // NOTE: There will always be at least one active process running
        // (i.e., this one)
        Iter first_proc = proc_map.begin();
        highest_usage = 
            std::pair<Key, int>(
                first_proc->first,
                get_usage(first_proc->second)
            );
    
        // Scan through rest of processes to see if one is higher
        for(Iter iter = proc_map.begin(); iter != proc_map.end(); ++iter) {
            if(iter->second.ram_usage > highest_usage.second) {
                highest_usage.first = iter->first;
                highest_usage.second = get_usage(iter->second);
            }
        }

        return proc_map.at(highest_usage.first);
    }

}

#endif
