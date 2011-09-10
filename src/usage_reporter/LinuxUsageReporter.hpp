#ifndef GUARD_LinuxUsageReporter_h
#define GUARD_LinuxUsageReporter_h

#include <functional>
#include <ios>

#include <unistd.h>

#include "FormattedProcessCollection.hpp"
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
        
        virtual std::string get_human_readable_name_for_processor_entry_(
            std::string& provided_name
        );

    private:
        void populate_stat_stream_(std::ostream& stat_stream);
};

#endif
