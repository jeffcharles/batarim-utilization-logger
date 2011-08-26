#ifndef GUARD_IActiveWindow_h
#define GUARD_IActiveWindow_h

#include <string>

class IActiveWindow
{
    public:    
        virtual ~IActiveWindow() { } 

        virtual std::string get_name() = 0;
        virtual std::string get_process_name() = 0;
        virtual int get_cpu_usage() = 0;
    
    protected:
        unsigned int pid_;

        std::string name_;
        std::string process_name_;

        IActiveWindow() { }
};

#endif
