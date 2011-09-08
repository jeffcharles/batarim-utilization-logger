#ifndef GUARD_WindowsUsageReporter_h
#define GUARD_WindowsUsageReporter_h

#include <string>
#include <vector>

#include <Pdh.h>
#include <Windows.h>

#include "RawProcessCollection.hpp"
#include "IUsageResultGetter.hpp"

#include "UsageReporter.hpp"

class WindowsUsageReporter : public UsageReporter
{
    protected:
        virtual bool initial_cpu_sweep_(PdhData&);
        virtual void pause_() { Sleep(1000); }
        virtual bool populate_processors_usage_(PdhData&);

        virtual void cleanup_processors_data_(
            PdhData& cpu_info
        ) {
            PdhCloseQuery(cpu_info.query);
        }

        virtual std::string get_human_readable_name_for_processor_entry_(
            std::string& provided_name
        );
};

#endif