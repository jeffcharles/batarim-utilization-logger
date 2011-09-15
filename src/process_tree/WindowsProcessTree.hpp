#ifndef GUARD_WindowsProcessTree_h
#define GUARD_WindowsProcessTree_h

#include "../formatted_process_collection/FormattedProcessCollection.hpp"

#include "ProcessTree.hpp"

class WindowsProcessTree : public ProcessTree
{
    protected:

        virtual bool process_is_top_level_(
            unsigned int pid,
            const FormattedProcessCollection& processes
        ) const;

};

#endif