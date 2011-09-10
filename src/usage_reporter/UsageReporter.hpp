#ifndef GUARD_UsageReporter_h
#define GUARD_UsageReporter_h

#include <memory>
#include <string>
#include <vector>

#include "IUsageResultGetter.hpp"
#include "FormattedProcessCollection.hpp"
#include "FormattedProcessCollectionFactory.hpp"
#include "../utilities/utilities.hpp"

#ifdef WIN32
#include <Pdh.h>
struct PdhData
{
    PDH_HQUERY query;
    PDH_HCOUNTER counter;
};
#define BATARIM_CPU_INFO_DATA_STRUCTURE PdhData
#endif

#ifdef LINUX
#include <sstream>
#define BATARIM_CPU_INFO_DATA_STRUCTURE std::stringstream
#endif

class UsageReporter : public IUsageResultGetter
{
    public:
        UsageReporter()
        {
            std::shared_ptr<std::vector<std::pair<std::string, int>>> pu(
                new std::vector<std::pair<std::string, int>>);
            processor_usages_ = pu;

            processes_ = get_formatted_process_collection();
        }

        // Analyzes CPU and RAM usages
        // Will take at least one second to complete
        // Should be called first from main method
        bool analyze(std::string& error_message);

        virtual const ProcessUsageInfo& 
        get_procinfo_for_highest_cpu_usage() const
        {
            return processes_->get_process_with_highest_cpu_usage();
        }

        virtual const ProcessUsageInfo&
        get_procinfo_for_highest_ram_usage() const
        {
            return processes_->get_process_with_highest_ram_usage();
        }

        virtual std::shared_ptr<std::vector<std::pair<std::string, int>>>
        get_processor_usages()
        {
            std::shared_ptr<std::vector<std::pair<std::string, int>>>
                proc_usage(processor_usages_);
            return proc_usage;
        }

        virtual const ProcessUsageInfo&
        get_process_usage(unsigned int pid) const
        {
            return processes_->get_process(pid);
        }
    
    protected:
        std::shared_ptr<std::vector<std::pair<std::string, int>>>
        processor_usages_;
        
        std::shared_ptr<FormattedProcessCollection> processes_;

        virtual bool initial_cpu_sweep_(BATARIM_CPU_INFO_DATA_STRUCTURE&) = 0;
        
        virtual void pause_() = 0;
        
        virtual bool populate_processors_usage_(
            BATARIM_CPU_INFO_DATA_STRUCTURE& initial_data
        ) = 0;
        
        virtual void cleanup_processors_data_(
            BATARIM_CPU_INFO_DATA_STRUCTURE& cpu_info
        ) = 0;
        
        virtual std::string get_human_readable_name_for_processor_entry_(
            std::string& provided_name
        ) = 0;
};

#endif
