#ifndef GUARD_AggregatedProcessCollection_h
#define GUARD_AggregatedProcessCollection_h

#include <map>
#include <string>

#include "../formatted_process_collection/FormattedProcessCollection.hpp"
#include "../formatted_process_collection/ProcessUsageInfo.hpp"

class AggregatedProcessCollection
{
    public:
        
        // NOTE: call after calling update on FormattedProcessCollection
        void update(const FormattedProcessCollection& processes);

        const ProcessUsageInfo& get_aggregate_with_highest_cpu_usage() const
        {
            return batarim::get_max_usage_in_map(
                aggregated_processes_,
                [](const ProcessUsageInfo& info) { return info.cpu_usage; }
            );
        }

        const ProcessUsageInfo& get_aggregate_with_highest_ram_usage() const
        {
            return batarim::get_max_usage_in_map(
                aggregated_processes_,
                [](const ProcessUsageInfo& info) { return info.ram_usage; }
            );
        }

    private:
        
        std::map<std::string, ProcessUsageInfo> aggregated_processes_;
};

#endif