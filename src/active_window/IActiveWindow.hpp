#ifndef GUARD_IActiveWindow_h
#define GUARD_IActiveWindow_h

#include <string>

#include "../usage_reporter/IUsageResultGetter.hpp"
#include "../usage_reporter/ProcessUsageInfo.hpp"

class IActiveWindow
{
    public:    
        virtual ~IActiveWindow() { } 

        virtual std::string get_name() = 0;
        virtual std::string get_process_name() = 0;

        const ProcessUsageInfo& get_usage(IUsageResultGetter& usage_results)
        {
            return usage_results.get_process_usage(pid_);
        }
    
    protected:
        unsigned int pid_;

        std::string name_;
        std::string process_name_;

        IActiveWindow() { }
};

#endif
