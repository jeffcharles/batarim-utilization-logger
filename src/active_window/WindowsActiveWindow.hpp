#ifndef GUARD_WindowsActiveWindow_h
#define GUARD_WindowsActiveWindow_h

#include <Windows.h>

#include <string>

#include "IActiveWindow.hpp"

class WindowsActiveWindow : public IActiveWindow
{
    public:
        WindowsActiveWindow();

        virtual std::wstring get_name() { return name_; }
        virtual std::wstring get_process_name();
        virtual int get_cpu_usage();

    private:
        std::wstring get_wstring_from_tchar(TCHAR* buffer, int buffer_size);
        std::wstring get_filename_from_win32_path(std::wstring& path);
        bool populate_filetimes(FILETIME& system_kernel_time1,
            FILETIME& system_user_time1, FILETIME& process_kernel_time1,
            FILETIME& process_user_time1, FILETIME& system_kernel_time2,
            FILETIME& system_user_time2, FILETIME& process_kernel_time2,
            FILETIME& process_user_time2);
        bool populate_filetimes(FILETIME& system_kernel_time,
            FILETIME& system_user_time, FILETIME& process_kernel_time,
            FILETIME& process_user_time);
        ULONGLONG get_ulonglong_from_filetime(FILETIME&);
};

#endif