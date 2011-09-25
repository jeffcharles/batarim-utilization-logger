#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "timestamp/timestamp.hpp"
#include "usage_reporter/UsageReporterFactory.hpp"
#include "usage_reporter/UsageReporter.hpp"
#include "formatted_process_collection/ProcessUsageInfo.hpp"
#include "ram_usage/ram_usage.hpp"
#include "active_window/active_window_module.hpp"
#include "active_window/IActiveWindow.hpp"
#include "uptime/uptime.hpp"

using std::cerr;
using std::clog;
using std::cout;
using std::endl;
using std::pair;
using std::shared_ptr;
using std::string;
using std::vector;

int main()
{
    shared_ptr<UsageReporter> usage_reporter = get_usage_reporter();

    string analyze_error_message;
    if(!usage_reporter->analyze(analyze_error_message)) {
        cerr << analyze_error_message << endl;
        clog << analyze_error_message << endl;
    }

    shared_ptr<vector<pair<string, int>>> cpu_usage_percentages =
        usage_reporter->get_processor_usages();

    shared_ptr<TimeInfo> time_info = get_time_info();

    int ram_usage = get_ram_usage();

    shared_ptr<IActiveWindow> active_window = get_active_window();
    string active_window_name = active_window->get_name();
    string active_window_process_name = active_window->get_process_name();
    ProcessUsageInfo active_window_usage =
        active_window->get_usage(*usage_reporter);

    ProcessUsageInfo highest_cpu_proc = 
        usage_reporter->get_procinfo_for_highest_cpu_usage();
    ProcessUsageInfo highest_ram_proc =
        usage_reporter->get_procinfo_for_highest_ram_usage();

    ProcessUsageInfo highest_cpu_aggregate =
        usage_reporter->get_procinfo_for_highest_aggregate_cpu_usage();
    ProcessUsageInfo highest_ram_aggregate =
        usage_reporter->get_procinfo_for_highest_aggregate_ram_usage();

    ProcessUsageInfo highest_cpu_toplevel =
        usage_reporter->get_procinfo_for_highest_toplevel_cpu_usage();
    ProcessUsageInfo highest_ram_toplevel =
        usage_reporter->get_procinfo_for_highest_aggregate_ram_usage();

    unsigned int uptime = get_uptime();
    
    cout << endl;

    cout << "Date and time: " << time_info->datetime_str << endl;
    cout << "Unix timestamp: " << time_info->timestamp << endl;

    cout << endl;

    cout << "Uptime (in seconds): " << uptime << endl;

    cout << endl;

    typedef vector<pair<string, int> >::const_iterator ConstIterator;
    for(ConstIterator iter = cpu_usage_percentages->begin();
        iter != cpu_usage_percentages->end(); iter++) {
        
        cout << "CPU " << iter->first << " usage: " << iter->second << "%" 
            << endl;
    }

    cout << endl;

    cout << "Current RAM usage: " << ram_usage << "%" << endl;

    cout << endl;

    cout << "Active window: " << active_window_name << endl;
    cout << "Active window process: " << active_window_process_name << endl;
    cout << "Active window CPU usage: " << active_window_usage.cpu_usage << "%"
        << endl;
    cout << "Active window RAM usage: " << active_window_usage.ram_usage << "%"
        << endl;
    
    cout << endl;
    
    cout << "Highest CPU usage process name: " 
        << highest_cpu_proc.process_name << endl;
    cout << "Highest CPU usage process CPU usage: "
        << highest_cpu_proc.cpu_usage << "%" << endl;
    cout << "Highest CPU usage process RAM usage: "
        << highest_cpu_proc.ram_usage << "%" << endl;

    cout << endl;

    cout << "Highest RAM usage process name: "
        << highest_ram_proc.process_name << endl;
    cout << "Highest RAM usage process CPU usage: "
        << highest_ram_proc.cpu_usage << "%" << endl;
    cout << "Highest RAM usage process RAM usage: "
        << highest_ram_proc.ram_usage << "%" << endl;

    cout << endl;

    cout << "Highest aggregate CPU usage process name: "
        << highest_cpu_aggregate.process_name << endl;
    cout << "Highest aggregate CPU usage process CPU usage: "
        << highest_cpu_aggregate.cpu_usage << "%" << endl;
    cout << "Highest aggregate CPU usage process RAM usage: "
        << highest_cpu_aggregate.ram_usage << "%" << endl;

    cout << endl;

    cout << "Highest aggregate RAM usage process name: "
        << highest_ram_aggregate.process_name << endl;
    cout << "Highest aggregate RAM usage process CPU usage: "
        << highest_ram_aggregate.cpu_usage << "%" << endl;
    cout << "Highest aggregate RAM usage process RAM usage: "
        << highest_ram_aggregate.ram_usage << "%" << endl;

    cout << endl;

    cout << "Highest top-level CPU usage process name: "
        << highest_cpu_toplevel.process_name << endl;
    cout << "Highest top-level CPU usage: "
        << highest_cpu_toplevel.cpu_usage << "%" << endl;
    cout << "Highest top-level CPU usage RAM usage: "
        << highest_cpu_toplevel.ram_usage << "%" << endl;

    cout << endl;

    cout << "Highest top-level RAM usage process name: "
        << highest_ram_toplevel.process_name << endl;
    cout << "Highest top-level RAM usage process CPU usage: "
        << highest_ram_toplevel.cpu_usage << "%" << endl;
    cout << "Highest top-level RAM usage: "
        << highest_ram_toplevel.ram_usage << "%" << endl;

    cout << endl;
}
