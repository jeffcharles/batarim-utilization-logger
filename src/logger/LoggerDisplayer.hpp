#ifndef GUARD_LoggerDisplayer_h
#define GUARD_LoggerDisplayer_h

#include "../viewmodel/IDisplayer.hpp"

class LoggerFieldNameDisplayer : public IDisplayer
{
    public:
        LoggerFieldNameDisplayer(): prefix('\0'), field_prefix("") { }

        virtual void display_external(
            const std::string& name,
            const std::shared_ptr<std::string> data_str
        ) {
            std::cout << prefix << '"';
            if(field_prefix != "") { 
                std::cout << field_prefix << ' ';
            }
            std::cout << name << '"'; 
            
            prefix = ',';
        }

        virtual void display_internal(
            const ViewModelInternalNode* node
        ) {
            field_prefix = node->name;

            typedef
                std::vector<std::shared_ptr<ViewModelElement>>::const_iterator
                ConstIter;
            for(ConstIter iter = node->children.begin();
                iter != node->children.end(); ++iter) {

                (*iter)->display();
            }

            field_prefix = "";
        }

    private:
        char prefix;
        std::string field_prefix;
};

class LoggerDataDisplayer : public IDisplayer
{
    public:
        LoggerDataDisplayer(): prefix('\0') { }

#ifdef WIN32
#pragma warning(disable: 4100)
#endif
// Disable unreferenced formal parameter warning since name is used in the
// other IDisplayers and we do not its value here
        virtual void display_external(
            const std::string& name,
            const std::shared_ptr<std::string> data_str
        ) {
            std::cout << prefix << '"' << *data_str << '"';
            prefix = ',';
        }
#ifdef WIN32
#pragma warning(default: 4100)
#endif

        virtual void display_internal(
            const ViewModelInternalNode* node
        ) {
            typedef
                std::vector<std::shared_ptr<ViewModelElement>>::const_iterator
                ConstIter;
            for(ConstIter iter = node->children.begin();
                iter != node->children.end(); ++iter) {

                (*iter)->display();
            }
        }

    private:
        char prefix;
};

#endif
