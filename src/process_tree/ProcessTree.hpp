#ifndef GUARD_ProcessTree_h
#define GUARD_ProcessTree_h

#include <map>
#include <unordered_map>

#include "../formatted_process_collection/FormattedProcessCollection.hpp"
#include "../formatted_process_collection/ProcessUsageInfo.hpp"

class ProcessTree
{
    public:
        
        void update(const FormattedProcessCollection& processes);
        
        const ProcessUsageInfo& get_highest_cpu_using_process_set() const
        {
            return batarim::get_max_usage_in_map(
                top_level_processes_,
                [](const ProcessUsageInfo& info) { return info.cpu_usage; }
            );
        }
        
        const ProcessUsageInfo& get_highest_ram_using_process_set() const
        {
            return batarim::get_max_usage_in_map(
                top_level_processes_,
                [](const ProcessUsageInfo& info) { return info.ram_usage; }
            );
        }

    protected:

        virtual bool process_is_top_level_(
            unsigned int pid,
            const FormattedProcessCollection& processes
        ) const = 0;

    private:
        
        std::map<unsigned int, ProcessUsageInfo> top_level_processes_;

        const unsigned int get_top_level_pid_(
            const unsigned int pid,
            const FormattedProcessCollection& processes,
            std::unordered_map<unsigned int, unsigned int>& lookup_table
        ) const;

        const unsigned int climb_tree_and_build_lookup_table_(
            const unsigned int pid,
            const FormattedProcessCollection& processes,
            std::unordered_map<unsigned int, unsigned int>& lookup_table
        ) const;
};

#endif