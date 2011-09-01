#ifndef GUARD_IUsageResultGetter_h
#define GUARD_IUsageResultGetter_h

#include <memory>
#include <string>
#include <vector>

class IUsageResultGetter
{
    public:
        virtual std::shared_ptr<std::vector<std::pair<std::string, int>>>
        get_processor_usages() = 0;

        virtual int get_process_cpu_usage(unsigned int pid) const = 0;
};

#endif