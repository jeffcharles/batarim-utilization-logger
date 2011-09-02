#ifndef GUARD_LinuxUsageReporter_h
#define GUARD_LinuxUsageReporter_h

#include <functional>
#include <ios>

#include <unistd.h>

#include "ProcessList.hpp"
#include "UsageReporter.hpp"

class LinuxUsageReporter : public UsageReporter
{
    protected:
        virtual bool initial_cpu_sweep_(std::stringstream& before_stream)
        {
            populate_stat_stream_(before_stream);
            return true;
        }
        
        virtual void pause_() { sleep(1); }
        
        virtual bool populate_processors_usage_(std::stringstream&);
        
        virtual void cleanup_processors_data_(std::stringstream&) { }
        
        virtual unsigned long long get_current_system_time_();
        
        virtual bool populate_process_list_(
            std::function<void(
                ProcessList&, unsigned int, unsigned long long)> set_time
        );

        virtual unsigned long long get_total_physical_ram_() const;

        virtual void populate_process_list_ram_();
        
        virtual std::string get_human_readable_name_for_processor_entry_(
            std::string& provided_name
        );

    private:
        void populate_stat_stream_(std::ostream& stat_stream);
        bool dir_entry_is_pid_(struct dirent* dir_entry, int* pid);
        unsigned long long get_process_cpu_time_(std::istream& stat_stream);
};

#endif
