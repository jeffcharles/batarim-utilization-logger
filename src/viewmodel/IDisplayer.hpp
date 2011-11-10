#ifndef GUARD_IDisplayer_h
#define GUARD_IDisplayer_h

#include <memory>

#include "DLLDefines.hpp"

class ViewModelInternalNode;

class VIEWMODEL_EXPORT IDisplayer
{
    public:
        virtual void display_external(
            const std::string& name,
            const std::unique_ptr<std::string> data_str
        ) = 0;
        
        virtual void display_internal(
            const ViewModelInternalNode*
        ) = 0;
};

#endif
