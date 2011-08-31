#ifndef GUARD_IActiveWindow_h
#define GUARD_IActiveWindow_h

#include <string>

#include "../usage_reporter/IUsageReporterRequestCollectionSetter.hpp"

class IActiveWindow
{
    public:    
        virtual ~IActiveWindow() { } 

        virtual std::string get_name() = 0;
        virtual std::string get_process_name() = 0;
        virtual void populate_cpu_usage(
            IUsageReporterRequestCollectionSetter& request_collection,
            int* cpu_usage
        ) = 0;
    
    protected:
        unsigned int pid_;

        std::string name_;
        std::string process_name_;

        IActiveWindow() { }
};

#endif
