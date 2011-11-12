#ifndef GUARD_UsageReporter_h
#define GUARD_UsageReporter_h

#include <memory>
#include <string>
#include <vector>

#include "IUsageResultGetter.hpp"
#include "../formatted_process_collection/FormattedProcessCollection.hpp"
#include "../formatted_process_collection/FormattedProcessCollectionFactory.hpp"
#include "../aggregated_process_collection/AggregatedProcessCollection.hpp"
#include "../process_tree/ProcessTree.hpp"
#include "../process_tree/process_tree_module.hpp"
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
            processes_ = get_formatted_process_collection();
            aggregated_processes_ = AggregatedProcessCollection();
            process_tree_ = get_process_tree();
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

        virtual const ProcessUsageInfo&
        get_procinfo_for_highest_aggregate_cpu_usage() const
        {
            return
                aggregated_processes_.get_aggregate_with_highest_cpu_usage();
        }

        virtual const ProcessUsageInfo&
        get_procinfo_for_highest_aggregate_ram_usage() const
        {
            return
                aggregated_processes_.get_aggregate_with_highest_ram_usage();
        }

        virtual const ProcessUsageInfo&
        get_procinfo_for_highest_toplevel_cpu_usage() const
        {
            return process_tree_->get_highest_cpu_using_process_set();
        }

        virtual const ProcessUsageInfo&
        get_procinfo_for_highest_toplevel_ram_usage() const
        {
            return process_tree_->get_highest_ram_using_process_set();
        }

        virtual const std::vector<std::pair<std::string, int>>&
        get_processor_usages()
        {
            return processor_usages_;
        }

        virtual const ProcessUsageInfo&
        get_process_usage(unsigned int pid) const
        {
            return processes_->get_process(pid);
        }
    
    protected:
        std::vector<std::pair<std::string, int>>
        processor_usages_;
        
        std::unique_ptr<FormattedProcessCollection> processes_;
        AggregatedProcessCollection aggregated_processes_;
        std::unique_ptr<ProcessTree> process_tree_;

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
