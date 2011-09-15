#include <string>

#include "UsageReporter.hpp"

using std::map;
using std::string;
using std::vector;

bool UsageReporter::analyze(string& error_message)
{
    BATARIM_CPU_INFO_DATA_STRUCTURE cpu_info;
    if(!initial_cpu_sweep_(cpu_info)) {
        error_message = "Initial CPU sweep failed";
        return false;
    }

    processes_->init();

    pause_();

    if(!populate_processors_usage_(cpu_info)) {
        error_message = "Populate processors usage failed";
        return false;
    }
    cleanup_processors_data_(cpu_info);
    
    processes_->update();
    aggregated_processes_.update(*processes_);
    process_tree_->update(*processes_);

    return true;
}