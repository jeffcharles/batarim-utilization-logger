#ifndef GUARD_active_window_h
#define GUARD_active_window_h

#include <string>
#include <Windows.h>

class ActiveWindow
{
    public:    
        ActiveWindow();

        std::wstring get_name() { return name_; }
        std::wstring get_process_name();
        int get_cpu_usage() { return cpu_usage_; }
    
    private:
        unsigned int pid_;

        std::wstring name_;
        std::wstring process_name_;
        int cpu_usage_;

        std::wstring get_wstring_from_tchar(TCHAR* buffer, int buffer_size);
        std::wstring get_filename_from_win32_path(std::wstring& path);
};

#endif
