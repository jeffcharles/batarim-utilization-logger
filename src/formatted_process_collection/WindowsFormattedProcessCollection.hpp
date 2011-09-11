#ifndef GUARD_WindowsFormattedProcessCollection_h
#define GUARD_WindowsFormattedProcessCollection_h

#include <Windows.h>

class WindowsFormattedProcessCollection : public FormattedProcessCollection
{
    protected:
        virtual unsigned long long get_current_system_time_() const
        {
            FILETIME ft_idle_time, ft_kernel_time, ft_user_time;
            GetSystemTimes(&ft_idle_time, &ft_kernel_time, &ft_user_time);
            unsigned long long kernel_time = convert_filetime_to_ulonglong_(
                ft_kernel_time);
            unsigned long long user_time = convert_filetime_to_ulonglong_(
                ft_user_time);
            return kernel_time + user_time;
        }

        virtual unsigned long long get_total_physical_ram_() const
        {
            MEMORYSTATUS memory_status;
            GlobalMemoryStatus(&memory_status);
            return memory_status.dwTotalPhys;
        }

    private:
        virtual unsigned long long convert_filetime_to_ulonglong_(
            FILETIME& filetime
        ) const {
            ULARGE_INTEGER uli;
            uli.HighPart = filetime.dwHighDateTime;
            uli.LowPart = filetime.dwLowDateTime;
            return uli.QuadPart;
        }
};

#endif