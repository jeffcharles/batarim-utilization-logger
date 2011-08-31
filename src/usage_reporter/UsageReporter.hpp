#ifndef GUARD_UsageReporter_h
#define GUARD_UsageReporter_h

#include <functional>
#include <string>
#include <vector>

#include "ProcessList.hpp"
#include "UsageReporterRequestCollection.hpp"

#ifdef WIN32
#include <Pdh.h>
struct PdhData
{
    PDH_HQUERY query;
    PDH_HCOUNTER counter;
};
#define BATARIM_CPU_INFO_DATA_STRUCTURE PdhData
#endif

class UsageReporter
{
    public:
        // Analyzes CPU and RAM usages and populates references
        // Will take at least one second to complete
        // Should only be called from main method
        bool Analyze(std::string& error_message);

        UsageReporterRequestCollection& get_request_collection()
        {
            return requests_;
        }
    
    protected:
        UsageReporterRequestCollection requests_;
        ProcessList process_list_;
        unsigned long long elapsed_system_time_;

        virtual bool initial_cpu_sweep_(BATARIM_CPU_INFO_DATA_STRUCTURE&) = 0;
        
        bool initial_populate_process_list_()
        {
            return populate_process_list_(&ProcessList::set_before_time);
        }
        
        virtual void pause_() = 0;
        virtual bool populate_processors_usage_(
            BATARIM_CPU_INFO_DATA_STRUCTURE& initial_data
        ) = 0;
        virtual void cleanup_processors_data_(
            BATARIM_CPU_INFO_DATA_STRUCTURE& cpu_info
        ) = 0;
        
        bool second_populate_process_list_()
        {
            return populate_process_list_(&ProcessList::set_after_time);
        }

        virtual unsigned long long get_current_system_time_() = 0;
        virtual bool populate_process_list_(
            std::function<void(
                ProcessList&, unsigned int, unsigned long long
                )> set_time
        ) = 0;
        virtual std::string get_human_readable_name_for_processor_entry_(
            std::string& provided_name
        ) = 0;

        std::vector<std::pair<std::string, int>>*
        get_processor_usages_result_()
        {
            return requests_.get_processors_usage_result();
        }
};

#endif
