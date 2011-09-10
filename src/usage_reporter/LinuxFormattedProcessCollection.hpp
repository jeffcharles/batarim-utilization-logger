#ifndef GUARD_LinuxFormattedProcessCollection_h
#define GUARD_LinuxFormattedProcessCollection_h

#include "FormattedProcessCollection.hpp"

class LinuxFormattedProcessCollection : public FormattedProcessCollection
{
    protected:

        virtual unsigned long long get_current_system_time_() const;

        virtual unsigned long long get_total_physical_ram_() const;
};

#endif
