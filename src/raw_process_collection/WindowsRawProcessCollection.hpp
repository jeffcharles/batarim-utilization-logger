#ifndef GUARD_WindowsRawProcessCollection_h
#define GUARD_WindowsRawProcessCollection_h

#include <memory>
#include <string>
#include <vector>

#include <Windows.h>

#include "../utilities/windows_utilities.hpp"

#include "RawProcessCollection.hpp"

class WindowsRawProcessCollection : public RawProcessCollection
{
    protected:

        virtual std::shared_ptr<std::vector<unsigned int>>
        get_pids_() const;

        virtual unsigned long long get_process_time_(unsigned int pid) const;

        virtual unsigned long long 
        get_process_ram_usage_(unsigned int pid) const;

        virtual void platform_specific_update_();

    private:

        unsigned long long convert_filetime_to_ulonglong_(FILETIME filetime) const
        {
            ULARGE_INTEGER uli;
            uli.HighPart = filetime.dwHighDateTime;
            uli.LowPart = filetime.dwLowDateTime;
            return uli.QuadPart;
        }

        std::shared_ptr<std::string>
        WindowsRawProcessCollection::get_process_exe_name_(
            const WCHAR* exe_file
        ) const {
            std::wstring full_file(exe_file);
            std::wstring base_file = (full_file.length() > 4) ?
                full_file.substr(0, full_file.length() - 4) :
                full_file;
            std::shared_ptr<std::string> utf8_base_file(new std::string( 
                batarim::convert_wstring_to_string(base_file)));
            return utf8_base_file;
        }
};

#endif