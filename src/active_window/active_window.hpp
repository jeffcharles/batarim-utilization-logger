#ifndef GUARD_active_window_h
#define GUARD_active_window_h

#include <string>

class active_window
{
    private:
        
        unsigned int pid;

        std::wstring name;
        std::wstring process_name;
        int cpu_usage;
    
    public:
        
        active_window();

        std::wstring get_name() { return name; }
        std::wstring get_process_name();
        int get_cpu_usage() { return cpu_usage; }
};

#endif
