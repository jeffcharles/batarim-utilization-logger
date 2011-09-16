#ifndef GUARD_LinuxProcessTree_h
#define GUARD_LinuxProcessTree_h

#include "../formatted_process_collection/FormattedProcessCollection.hpp"

#include "ProcessTree.hpp"

class LinuxProcessTree : public ProcessTree
{
    protected:
        virtual bool process_is_top_level_(
            unsigned int pid,
            const FormattedProcessCollection& processes
        ) const {
            
            unsigned int ppid = processes.get_ppid(pid);

            // PID 0 is init and kthreadd's parent
            // PID 1 is init
            // PID 2 is kthreadd
            return (ppid >= 0 && ppid <= 2);
        }
};

#endif
