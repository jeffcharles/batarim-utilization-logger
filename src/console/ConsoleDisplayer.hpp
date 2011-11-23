#ifndef GUARD_ConsoleDisplayer_h
#define GUARD_ConsoleDisplayer_h

#include <cstdio>
#include <iostream>
#include <memory>
#include <vector>

#include "../viewmodel/IDisplayer.hpp"
#include "../viewmodel/viewmodel.hpp"

class ConsoleDisplayer : public IDisplayer
{
    public:
        virtual void display_external(
            const std::string& name,
            const std::unique_ptr<std::string> data_str
        ) {
            std::cout << name << ": " << *data_str << std::endl;
        }

        void display_internal(
            const ViewModelInternalNode* node
        ) {
            std::cout << node->name << ':' << std::endl;

            for(ViewModelInternalNode::const_iterator iter = node->begin();
                iter != node->end(); ++iter) {

                (*iter)->display(this);
            }

            std::cout << std::endl;
        }
};

#endif
