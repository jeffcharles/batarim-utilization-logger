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
        virtual int get_cpu_usage() { return cpu_usage_; }

    private:
        std::wstring get_wstring_from_tchar(TCHAR* buffer, int buffer_size);
        std::wstring get_filename_from_win32_path(std::wstring& path);
};

#endif