#ifndef GUARD_LoggerDisplayer_h
#define GUARD_LoggerDisplayer_h

#include "../viewmodel/IDisplayer.hpp"

class LoggerFieldNameDisplayer : public IDisplayer
{
    public:
        LoggerFieldNameDisplayer(std::ofstream& log_file):
            prefix('\0'), field_prefix(""), log_file_(log_file) { }

        virtual void display_external(
            const std::string& name,
            const std::shared_ptr<std::string> data_str
        ) {
            log_file_ << prefix << '"';
            if(field_prefix != "") { 
                log_file_ << field_prefix << ' ';
            }
            log_file_ << name << '"'; 
            
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
        std::ofstream& log_file_;
};

class LoggerDataDisplayer : public IDisplayer
{
    public:
        LoggerDataDisplayer(std::ofstream& log_file):
            prefix('\0'), log_file_(log_file) { }

#ifdef WIN32
#pragma warning(disable: 4100)
#endif
// Disable unreferenced formal parameter warning since name is used in the
// other IDisplayers and we do not its value here
        virtual void display_external(
            const std::string& name,
            const std::shared_ptr<std::string> data_str
        ) {
            log_file_ << prefix << '"' << *data_str << '"';
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
        std::ofstream& log_file_;
};

#endif
