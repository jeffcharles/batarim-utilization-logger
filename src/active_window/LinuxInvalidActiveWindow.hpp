#ifndef GUARD_LinuxInvalidActiveWindow_h
#define GUARD_LinuxInvalidActiveWindow_h

#include "../usage_reporter/IUsageResultGetter.hpp"

class LinuxInvalidActiveWindow : public IActiveWindow
{
    public:
        LinuxInvalidActiveWindow()
        {
            name_ = "";
            process_name_ = "";
        }

        virtual std::string get_name() { return name_; }
        virtual std::string get_process_name() { return process_name_; }
        
        virtual
        const ProcessUsageInfo& get_usage(IUsageResultGetter& usage_results)
        {
            info_.process_name = "";
            info_.cpu_usage = 0;
            info_.ram_usage = 0;
            return info_;
        }
    
    private:
        ProcessUsageInfo info_;
};

#endif
