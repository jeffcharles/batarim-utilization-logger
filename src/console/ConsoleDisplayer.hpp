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
            const std::shared_ptr<std::string> data_str
        ) {
            std::cout << name << ": " << *data_str << std::endl;
        }

        void display_internal(
            const ViewModelInternalNode* node
        ) {
            std::cout << node->name << ':' << std::endl;

            typedef
                std::vector<std::shared_ptr<ViewModelElement>>::const_iterator
                ConstIter;
            for(ConstIter iter = node->children.begin();
                iter != node->children.end(); ++iter) {

                (*iter)->display();
            }
        }
};

#endif
