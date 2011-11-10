#include <ios>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "../timestamp/timestamp.hpp"
#include "../usage_reporter/UsageReporterFactory.hpp"
#include "../usage_reporter/UsageReporter.hpp"
#include "../formatted_process_collection/ProcessUsageInfo.hpp"
#include "../ram_usage/ram_usage.hpp"
#include "../active_window/active_window_module.hpp"
#include "../active_window/IActiveWindow.hpp"
#include "../uptime/uptime.hpp"
#include "IDisplayer.hpp"

#include "viewmodel.hpp"

using std::cerr;
using std::clog;
using std::endl;
using std::pair;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::vector;

namespace {

    shared_ptr<ViewModelElement> convert_process_usage_to_element(
        const string field_group_name,
        const ProcessUsageInfo& info,
        shared_ptr<IDisplayer> displayer
    ) {
        shared_ptr<ViewModelElement> process_name(
            new ViewModelExternalNode<string>(
                "Process name",
                info.process_name,
                displayer
            )
        );
        
        shared_ptr<ViewModelElement> cpu_usage(
            new ViewModelExternalNode<int>(
                "CPU usage",
                info.cpu_usage,
                displayer
            )
        );

        shared_ptr<ViewModelElement> ram_usage(
            new ViewModelExternalNode<int>(
                "RAM usage",
                info.ram_usage,
                displayer
            )
        );

        vector<shared_ptr<ViewModelElement>> elements;
        elements.push_back(process_name);
        elements.push_back(cpu_usage);
        elements.push_back(ram_usage);

        shared_ptr<ViewModelInternalNode> ret(
            new ViewModelInternalNode(
                field_group_name,
                elements,
                displayer
            )
        );

        return ret;
    }
}

unique_ptr<vector<shared_ptr<ViewModelElement>>> get_view_model(
    shared_ptr<IDisplayer> displayer
) {
    shared_ptr<UsageReporter> usage_reporter = get_usage_reporter();

    string analyze_error_message;
    if(!usage_reporter->analyze(analyze_error_message)) {
        cerr << analyze_error_message << endl;
        clog << analyze_error_message << endl;
    }

    unique_ptr<vector<shared_ptr<ViewModelElement>>> viewmodel(
        new vector<shared_ptr<ViewModelElement>>
    );

    shared_ptr<TimeInfo> time_info = get_time_info();
    shared_ptr<ViewModelElement> datetime_str(
        new ViewModelExternalNode<string>(
            "Date and time",
            time_info->datetime_str,
            displayer
        )
    );
    shared_ptr<ViewModelElement> timestamp(
        new ViewModelExternalNode<time_t>(
            "Unix timestamp",
            time_info->timestamp,
            displayer
        )
    );
    vector<shared_ptr<ViewModelElement>> time_node_contents;
    time_node_contents.push_back(datetime_str);
    time_node_contents.push_back(timestamp);
    shared_ptr<ViewModelElement> time_node(
        new ViewModelInternalNode(
            "Time information",
            time_node_contents,
            displayer
        )
    );
    viewmodel->push_back(time_node);

    unsigned int uptime = get_uptime();
    shared_ptr<ViewModelElement> uptime_node(
        new ViewModelExternalNode<unsigned int>(
            "Uptime",
            uptime,
            displayer
        )
    );
    viewmodel->push_back(uptime_node);
    
    shared_ptr<vector<pair<string, int>>> cpu_usage_percentages =
        usage_reporter->get_processor_usages();
    vector<shared_ptr<ViewModelElement>> cpu_usages_content;
    typedef vector<pair<string, int>>::const_iterator ConstIter;
    for(ConstIter iter = cpu_usage_percentages->begin();
        iter != cpu_usage_percentages->end(); ++iter) {
        
        stringstream cpu_name;
        cpu_name << "CPU " << iter->first << " usage";
        shared_ptr<ViewModelElement> cpu_usage_node(
            new ViewModelExternalNode<int>(
                cpu_name.str(),
                iter->second,
                displayer
            )
        );
        cpu_usages_content.push_back(cpu_usage_node);
    }
    shared_ptr<ViewModelElement> cpu_usages(
        new ViewModelInternalNode(
            "CPU usages",
            cpu_usages_content,
            displayer
        )
    );
    viewmodel->push_back(cpu_usages);
    
    int ram_usage = get_ram_usage();
    shared_ptr<ViewModelElement> ram_usage_node(
        new ViewModelExternalNode<int>(
            "RAM usage",
            ram_usage,
            displayer
        )
    );
    viewmodel->push_back(ram_usage_node);

    shared_ptr<IActiveWindow> active_window = get_active_window();
    string active_window_name = active_window->get_name();
    string active_window_process_name = active_window->get_process_name();
    ProcessUsageInfo active_window_usage =
        active_window->get_usage(*usage_reporter);
    shared_ptr<ViewModelElement> active_window_name_node(
        new ViewModelExternalNode<string>(
            "Name",
            active_window_name,
            displayer
        )
    );
    shared_ptr<ViewModelElement> active_window_process_name_node(
        new ViewModelExternalNode<string>(
            "Process name",
            active_window_process_name,
            displayer
        )
    );
    shared_ptr<ViewModelElement> active_window_cpu_node(
        new ViewModelExternalNode<int>(
            "CPU usage",
            active_window_usage.cpu_usage,
            displayer
        )
    );
    shared_ptr<ViewModelElement> active_window_ram_node(
        new ViewModelExternalNode<int>(
            "RAM usage",
            active_window_usage.ram_usage,
            displayer
        )
    );
    vector<shared_ptr<ViewModelElement>> active_window_node_content;
    active_window_node_content.push_back(active_window_name_node);
    active_window_node_content.push_back(active_window_process_name_node);
    active_window_node_content.push_back(active_window_cpu_node);
    active_window_node_content.push_back(active_window_ram_node);
    shared_ptr<ViewModelInternalNode> active_window_node(
        new ViewModelInternalNode(
            "Focused window",
            active_window_node_content,
            displayer
        )
    );
    viewmodel->push_back(active_window_node);

    ProcessUsageInfo highest_cpu_proc = 
        usage_reporter->get_procinfo_for_highest_cpu_usage();
    ProcessUsageInfo highest_ram_proc =
        usage_reporter->get_procinfo_for_highest_ram_usage();
    viewmodel->push_back(convert_process_usage_to_element(
        "Highest CPU usage process",
        highest_cpu_proc,
        displayer
    ));
    viewmodel->push_back(convert_process_usage_to_element(
        "Highest RAM usage process",
        highest_ram_proc,
        displayer
    ));

    ProcessUsageInfo highest_cpu_aggregate =
        usage_reporter->get_procinfo_for_highest_aggregate_cpu_usage();
    ProcessUsageInfo highest_ram_aggregate =
        usage_reporter->get_procinfo_for_highest_aggregate_ram_usage();
    viewmodel->push_back(convert_process_usage_to_element(
        "Highest aggregate CPU usage process",
        highest_cpu_aggregate,
        displayer
    ));
    viewmodel->push_back(convert_process_usage_to_element(
        "Highest aggregate RAM usage process",
        highest_ram_aggregate,
        displayer
    ));

    ProcessUsageInfo highest_cpu_toplevel =
        usage_reporter->get_procinfo_for_highest_toplevel_cpu_usage();
    ProcessUsageInfo highest_ram_toplevel =
        usage_reporter->get_procinfo_for_highest_aggregate_ram_usage();
    viewmodel->push_back(convert_process_usage_to_element(
        "Highest top-level CPU usage process",
        highest_cpu_toplevel,
        displayer
    ));
    viewmodel->push_back(convert_process_usage_to_element(
        "Highest top-level RAM usage process",
        highest_ram_toplevel,
        displayer
    ));

    return viewmodel;
}
