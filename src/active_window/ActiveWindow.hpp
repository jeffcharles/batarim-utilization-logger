#ifndef GUARD_active_window_h
#define GUARD_active_window_h

#include <string>

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
};

#endif
