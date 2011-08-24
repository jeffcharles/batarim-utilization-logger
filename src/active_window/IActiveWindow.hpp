#ifndef GUARD_IActiveWindow_h
#define GUARD_IActiveWindow_h

#include <string>

class IActiveWindow
{
    public:    
        virtual ~IActiveWindow() { } 

        virtual std::wstring get_name() = 0;
        virtual std::wstring get_process_name() = 0;
        virtual int get_cpu_usage() = 0;
    
    protected:
        unsigned int pid_;

        std::wstring name_;
        std::wstring process_name_;

        IActiveWindow() { }
};

#endif
