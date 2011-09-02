#ifndef GUARD_IActiveWindow_h
#define GUARD_IActiveWindow_h

#include <string>

#include "../usage_reporter/IUsageResultGetter.hpp"

class IActiveWindow
{
    public:    
        virtual ~IActiveWindow() { } 

        virtual std::string get_name() = 0;
        virtual std::string get_process_name() = 0;
        
        virtual int get_cpu_usage(IUsageResultGetter& usage_results)
        {
            return usage_results.get_process_cpu_usage(pid_);
        }

        virtual int get_ram_usage(IUsageResultGetter& usage_results)
        {
            return usage_results.get_process_ram_usage(pid_);
        }
    
    protected:
        unsigned int pid_;

        std::string name_;
        std::string process_name_;

        IActiveWindow() { }
};

#endif
