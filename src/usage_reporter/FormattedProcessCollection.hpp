#ifndef GUARD_FormattedPerformanceCollection_h
#define GUARD_FormattedPerformanceCollection_h

#include <memory>

#include "../utilities/utilities.hpp"
#include "ProcessUsageInfo.hpp"
#include "RawProcessCollection.hpp"
#include "RawProcessCollectionFactory.hpp"

class FormattedProcessCollection
{
    public:

        FormattedProcessCollection()
        {
            processes_ = get_raw_process_collection();
        }

        // NOTE: must call first init followed by update after a one
        // second delay
        void init()
        {
            processes_->init();
            before_system_time_ = get_current_system_time_();
        }

        void update()
        {
            processes_->update();
            elapsed_system_time_ =
                get_current_system_time_() - before_system_time_;
            total_physical_ram_ = get_total_physical_ram_();
        }

        std::shared_ptr<ProcessUsageInfo> get_process(unsigned int pid) const
        {
            std::shared_ptr<ProcessUsageInfo> proc_info(new ProcessUsageInfo);
            proc_info->process_name = batarim::get_process_name(pid);
            unsigned long long elapsed_time = processes_->get_time(pid);
            proc_info->cpu_usage = calculate_cpu_usage_(elapsed_time);
            unsigned long long ram_bytes_used = processes_->get_ram(pid);
            proc_info->ram_usage = calculate_ram_usage_(ram_bytes_used);
            return proc_info;
        }
        
        std::shared_ptr<ProcessUsageInfo>
        get_process_with_highest_cpu_usage() const
        {
            unsigned int pid = processes_->get_pid_with_highest_cpu_usage();
            return get_process(pid);
        }

        std::shared_ptr<ProcessUsageInfo>
        get_process_with_highest_ram_usage() const
        {
            unsigned int pid = processes_->get_pid_with_highest_ram_usage();
            return get_process(pid);
        }

    protected:

        virtual unsigned long long get_current_system_time_() const = 0;
        virtual unsigned long long get_total_physical_ram_() const = 0;

    private:
        
        std::shared_ptr<RawProcessCollection> processes_;

        unsigned long long before_system_time_;
        unsigned long long elapsed_system_time_;
        unsigned long long total_physical_ram_;

        int calculate_cpu_usage_(unsigned long long elapsed_process_time) const
        {
            int cpu_usage = (int)((double)elapsed_process_time /
                elapsed_system_time_ * 100);
            return cpu_usage;
        }

        int calculate_ram_usage_(unsigned long long ram_bytes_used) const
        {
            int ram_usage = (int)((double)ram_bytes_used /
                total_physical_ram_ * 100);
            return ram_usage;
        }
};

#endif
