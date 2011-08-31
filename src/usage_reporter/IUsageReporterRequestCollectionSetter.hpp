#ifndef GUARD_IUsageReporterRequestCollectionSetter_h
#define GUARD_IUsageReporterRequestCollectionSetter_h

#include <string>
#include <vector>

class IUsageReporterRequestCollectionSetter
{
    public:
       
        virtual void add_specific_process_cpu_request(
            unsigned int pid,
            int* cpu_result
        ) = 0;

        virtual void add_specific_process_ram_request(
            unsigned int pid,
            int* ram_result
        ) = 0;
        
        virtual void add_procesors_usage_request(
            std::vector<std::pair<std::string, int>>* result
        ) = 0;
};

#endif