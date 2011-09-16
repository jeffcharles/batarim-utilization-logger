#ifndef GUARD_LinuxRawProcessCollection_h
#define GUARD_LinuxRawProcessCollection_h

#include <memory>
#include <vector>

#include "RawProcessCollection.hpp"

class LinuxRawProcessCollection : public RawProcessCollection
{
    protected:
        
        virtual std::shared_ptr<std::vector<unsigned int>> get_pids_() const;

        virtual unsigned long long get_process_time_(unsigned int pid) const;

        virtual unsigned long long
        get_process_ram_usage_(unsigned int pid) const;

        virtual void platform_specific_update_();

    private:

        unsigned int get_process_ppid_(unsigned int pid) const;
};

#endif
