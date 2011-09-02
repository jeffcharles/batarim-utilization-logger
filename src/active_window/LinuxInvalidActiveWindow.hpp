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
        
        virtual int get_cpu_usage(IUsageResultGetter& usage_results)
        {
            return 0;
        }

        virtual int get_ram_usage(IUsageResultGetter& usage_results)
        {
            return 0;
        }
};

#endif
