#ifndef GUARD_WindowsUsageReporter_h
#define GUARD_WindowsUsageReporter_h

#include <functional>
#include <string>
#include <vector>

#include <Pdh.h>
#include <Windows.h>

#include "ProcessList.hpp"
#include "IUsageResultGetter.hpp"

#include "UsageReporter.hpp"

class WindowsUsageReporter : public UsageReporter
{
    public:

        virtual std::shared_ptr<ProcessUsageInfo>
        get_procinfo_for_highest_cpu_usage() const;

        virtual std::shared_ptr<ProcessUsageInfo>
        get_procinfo_for_highest_ram_usage() const;

    protected:
        virtual bool initial_cpu_sweep_(PdhData&);
        virtual void pause_() { Sleep(1000); }
        virtual bool populate_processors_usage_(PdhData&);

        virtual void cleanup_processors_data_(
            PdhData& cpu_info
        ) {
            PdhCloseQuery(cpu_info.query);
        }

        virtual unsigned long long get_current_system_time_()
        {
            FILETIME ft_idle_time, ft_kernel_time, ft_user_time;
            GetSystemTimes(&ft_idle_time, &ft_kernel_time, &ft_user_time);
            unsigned long long kernel_time = convert_filetime_to_ulonglong_(
                ft_kernel_time);
            unsigned long long user_time = convert_filetime_to_ulonglong_(
                ft_user_time);
            return kernel_time + user_time;
        }

        virtual bool populate_process_list_(
            std::function<void(
                ProcessList&, unsigned int, unsigned long long
                )> set_time
        );

        virtual unsigned long long get_total_physical_ram_() const
        {
            MEMORYSTATUS memory_status;
            GlobalMemoryStatus(&memory_status);
            return memory_status.dwTotalPhys;
        }

        virtual void populate_process_list_ram_();

        virtual std::string get_human_readable_name_for_processor_entry_(
            std::string& provided_name
        );

    private:   
        
        unsigned long long 
        WindowsUsageReporter::convert_filetime_to_ulonglong_(
            FILETIME& filetime
        ) {
            ULARGE_INTEGER uli;
            uli.HighPart = filetime.dwHighDateTime;
            uli.LowPart = filetime.dwLowDateTime;
            return uli.QuadPart;
        }
};

#endif