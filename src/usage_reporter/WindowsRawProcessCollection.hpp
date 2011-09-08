#ifndef GUARD_WindowsRawProcessCollection_h
#define GUARD_WindowsRawProcessCollection_h

#include <memory>
#include <vector>

#include <Windows.h>

#include "RawProcessCollection.hpp"

class WindowsRawProcessCollection : public RawProcessCollection
{
    protected:

        virtual std::shared_ptr<std::vector<unsigned int>>
        get_pids_() const;

        virtual unsigned long long get_process_time_(unsigned int pid) const;

        virtual unsigned long long 
        get_process_ram_usage_(unsigned int pid) const;

    private:

        unsigned long long convert_filetime_to_ulonglong_(FILETIME filetime) const
        {
            ULARGE_INTEGER uli;
            uli.HighPart = filetime.dwHighDateTime;
            uli.LowPart = filetime.dwLowDateTime;
            return uli.QuadPart;
        }
};

#endif