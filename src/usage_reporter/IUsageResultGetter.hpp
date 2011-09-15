#ifndef GUARD_IUsageResultGetter_h
#define GUARD_IUsageResultGetter_h

#include <memory>
#include <string>
#include <vector>

#include "../formatted_process_collection/ProcessUsageInfo.hpp"

class IUsageResultGetter
{
    public:
        virtual const ProcessUsageInfo&
        get_procinfo_for_highest_cpu_usage() const = 0;

        virtual const ProcessUsageInfo&
        get_procinfo_for_highest_ram_usage() const = 0;

        virtual const ProcessUsageInfo&
        get_procinfo_for_highest_aggregate_cpu_usage() const = 0;

        virtual const ProcessUsageInfo&
        get_procinfo_for_highest_aggregate_ram_usage() const = 0;

        virtual const ProcessUsageInfo&
        get_procinfo_for_highest_toplevel_cpu_usage() const = 0;

        virtual const ProcessUsageInfo&
        get_procinfo_for_highest_toplevel_ram_usage() const = 0;

        virtual std::shared_ptr<std::vector<std::pair<std::string, int>>>
        get_processor_usages() = 0;

        virtual const ProcessUsageInfo&
        get_process_usage(unsigned int pid) const = 0;
};

#endif