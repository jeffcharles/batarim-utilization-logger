#ifndef GUARD_UsageReporterRequestCollection_h
#define GUARD_UsageReporterRequestCollection_h

#include <map>
#include <string>
#include <vector>

#include "IUsageReporterRequestCollectionSetter.hpp"
#include "ProcessInfoOutputs.hpp"

class UsageReporterRequestCollection :
    public IUsageReporterRequestCollectionSetter
{
    public:
        
        virtual void add_specific_process_cpu_request(
            unsigned int pid,
            int* cpu_result
        ) {
            specific_process_requests_[pid].cpu_result = cpu_result;
        }

        virtual void add_specific_process_ram_request(
            unsigned int pid,
            int* ram_result
        ) {
            specific_process_requests_[pid].ram_result = ram_result;
        }
        
        virtual void add_procesors_usage_request(
            std::vector<std::pair<std::string, int>>* result)
        {
            processors_usage_result_ = result;
        }

        std::map<unsigned int, ProcessInfoOutputs>
        get_specific_process_requests()
        {
            return specific_process_requests_;
        }

        std::vector<std::pair<std::string, int>>*
        get_processors_usage_result()
        {
            return processors_usage_result_;
        }

    private:
        std::map<unsigned int, ProcessInfoOutputs> specific_process_requests_;
        std::vector<std::pair<std::string, int>>* processors_usage_result_;
};

#endif
