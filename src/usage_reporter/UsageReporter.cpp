#include <map>
#include <string>
#include <vector>

#include "UsageReporter.hpp"

using std::map;
using std::string;
using std::vector;

bool UsageReporter::analyze(string& error_message)
{
    // Populate process lists and processors usage result
    BATARIM_CPU_INFO_DATA_STRUCTURE cpu_info;
    if(!initial_cpu_sweep_(cpu_info)) {
        error_message = "Initial CPU sweep failed";
        return false;
    }
    unsigned long long initial_system_time = get_current_system_time_();
    if(!initial_populate_process_list_()) {
        error_message = "Initial populate process list failed";
        return false;
    }
    pause_();
    if(!populate_processors_usage_(cpu_info)) {
        error_message = "Populate processors usage failed";
        return false;
    }
    cleanup_processors_data_(cpu_info);
    unsigned long long second_system_time = get_current_system_time_();
    elapsed_system_time_ = second_system_time - initial_system_time;
    if(!second_populate_process_list_()) {
        error_message = "Second call to populate process list failed";
        return false;
    }

    return true;
}