#ifndef GUARD_IUsageResultGetter_h
#define GUARD_IUsageResultGetter_h

#include <memory>
#include <string>
#include <vector>

#include "ProcessUsageInfo.hpp"

class IUsageResultGetter
{
    public:
        virtual std::shared_ptr<ProcessUsageInfo>
        get_procinfo_for_highest_cpu_usage() const = 0;

        virtual std::shared_ptr<ProcessUsageInfo>
        get_procinfo_for_highest_ram_usage() const = 0;

        virtual std::shared_ptr<std::vector<std::pair<std::string, int>>>
        get_processor_usages() = 0;

        virtual int get_process_cpu_usage(unsigned int pid) const = 0;

        virtual int get_process_ram_usage(unsigned int pid) const = 0;
};

#endif