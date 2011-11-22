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

    unique_ptr<ViewModelInternalNode> convert_process_usage_to_element(
        const string field_group_name,
        const ProcessUsageInfo& info
    ) {
        unique_ptr<ViewModelElement> process_name(
            new ViewModelExternalNode<string>(
                "Process name",
                info.process_name
            )
        );
        
        unique_ptr<ViewModelElement> cpu_usage(
            new ViewModelExternalNode<int>(
                "CPU usage",
                info.cpu_usage
            )
        );

        unique_ptr<ViewModelElement> ram_usage(
            new ViewModelExternalNode<int>(
                "RAM usage",
                info.ram_usage
            )
        );

        vector<unique_ptr<ViewModelElement>> elements;
        elements.push_back(move(process_name));
        elements.push_back(move(cpu_usage));
        elements.push_back(move(ram_usage));

        unique_ptr<ViewModelInternalNode> ret(
            new ViewModelInternalNode(
                field_group_name,
                move(elements)
            )
        );

        return move(ret);
    }
}

unique_ptr<IViewModelViewer> get_view_model()
{
    unique_ptr<UsageReporter> usage_reporter = get_usage_reporter();

    string analyze_error_message;
    if(!usage_reporter->analyze(analyze_error_message)) {
        cerr << analyze_error_message << endl;
        clog << analyze_error_message << endl;
    }

    unique_ptr<ViewModel> viewmodel(new ViewModel());

    unique_ptr<TimeInfo> time_info = get_time_info();
    unique_ptr<ViewModelElement> datetime_str(
        new ViewModelExternalNode<string>(
            "Date and time",
            time_info->datetime_str
        )
    );
    unique_ptr<ViewModelElement> timestamp(
        new ViewModelExternalNode<time_t>(
            "Unix timestamp",
            time_info->timestamp
        )
    );
    vector<unique_ptr<ViewModelElement>> time_node_contents;
    time_node_contents.push_back(move(datetime_str));
    time_node_contents.push_back(move(timestamp));
    unique_ptr<ViewModelInternalNode> time_node(
        new ViewModelInternalNode(
            "Time information",
            move(time_node_contents)
        )
    );
    viewmodel->push_back(move(time_node));

    unsigned int uptime = get_uptime();
    unique_ptr<ViewModelElement> uptime_node(
        new ViewModelExternalNode<unsigned int>(
            "Uptime",
            uptime
        )
    );
    vector<unique_ptr<ViewModelElement>> uptime_node_container_contents;
    uptime_node_container_contents.push_back(move(uptime_node));
    unique_ptr<ViewModelInternalNode> uptime_node_container(
        new ViewModelInternalNode(
            "",
            move(uptime_node_container_contents)
        )
    );
    viewmodel->push_back(move(uptime_node_container));
    
    vector<pair<string, int>> cpu_usage_percentages =
        usage_reporter->get_processor_usages();
    vector<unique_ptr<ViewModelElement>> cpu_usages_content;
    typedef vector<pair<string, int>>::const_iterator ConstIter;
    for(ConstIter iter = cpu_usage_percentages.begin();
        iter != cpu_usage_percentages.end(); ++iter) {
        
        stringstream cpu_name;
        cpu_name << "CPU " << iter->first << " usage";
        unique_ptr<ViewModelElement> cpu_usage_node(
            new ViewModelExternalNode<int>(
                cpu_name.str(),
                iter->second
            )
        );
        cpu_usages_content.push_back(move(cpu_usage_node));
    }
    unique_ptr<ViewModelInternalNode> cpu_usages(
        new ViewModelInternalNode(
            "CPU usages",
            move(cpu_usages_content)
        )
    );
    viewmodel->push_back(move(cpu_usages));
    
    int ram_usage = get_ram_usage();
    unique_ptr<ViewModelElement> ram_usage_node(
        new ViewModelExternalNode<int>(
            "RAM usage",
            ram_usage
        )
    );
    vector<unique_ptr<ViewModelElement>> ram_usage_container_contents;
    ram_usage_container_contents.push_back(move(ram_usage_node));
    unique_ptr<ViewModelInternalNode> ram_usage_container(
        new ViewModelInternalNode(
            "",
            move(ram_usage_container_contents)
        )
    );
    viewmodel->push_back(move(ram_usage_container));

    unique_ptr<IActiveWindow> active_window = get_active_window();
    string active_window_name = active_window->get_name();
    string active_window_process_name = active_window->get_process_name();
    ProcessUsageInfo active_window_usage =
        active_window->get_usage(*usage_reporter);
    unique_ptr<ViewModelElement> active_window_name_node(
        new ViewModelExternalNode<string>(
            "Name",
            active_window_name
        )
    );
    unique_ptr<ViewModelElement> active_window_process_name_node(
        new ViewModelExternalNode<string>(
            "Process name",
            active_window_process_name
        )
    );
    unique_ptr<ViewModelElement> active_window_cpu_node(
        new ViewModelExternalNode<int>(
            "CPU usage",
            active_window_usage.cpu_usage
        )
    );
    unique_ptr<ViewModelElement> active_window_ram_node(
        new ViewModelExternalNode<int>(
            "RAM usage",
            active_window_usage.ram_usage
        )
    );
    vector<unique_ptr<ViewModelElement>> active_window_node_content;
    active_window_node_content.push_back(move(active_window_name_node));
    active_window_node_content.push_back(move(
        active_window_process_name_node));
    active_window_node_content.push_back(move(active_window_cpu_node));
    active_window_node_content.push_back(move(active_window_ram_node));
    unique_ptr<ViewModelInternalNode> active_window_node(
        new ViewModelInternalNode(
            "Focused window",
            move(active_window_node_content)
        )
    );
    viewmodel->push_back(move(active_window_node));

    ProcessUsageInfo highest_cpu_proc = 
        usage_reporter->get_procinfo_for_highest_cpu_usage();
    ProcessUsageInfo highest_ram_proc =
        usage_reporter->get_procinfo_for_highest_ram_usage();
    unique_ptr<ViewModelInternalNode> highest_cpu_element =
        convert_process_usage_to_element(
            "Highest CPU usage process",
            highest_cpu_proc
        );
    unique_ptr<ViewModelInternalNode> highest_ram_element =
        convert_process_usage_to_element(
            "Highest RAM usage process",
            highest_ram_proc
        );
    viewmodel->push_back(move(highest_cpu_element));
    viewmodel->push_back(move(highest_ram_element));

    ProcessUsageInfo highest_cpu_aggregate =
        usage_reporter->get_procinfo_for_highest_aggregate_cpu_usage();
    ProcessUsageInfo highest_ram_aggregate =
        usage_reporter->get_procinfo_for_highest_aggregate_ram_usage();
    unique_ptr<ViewModelInternalNode> highest_cpu_aggregate_element =
        convert_process_usage_to_element(
            "Highest aggregate CPU usage process",
            highest_cpu_aggregate
        );
    unique_ptr<ViewModelInternalNode> highest_ram_aggregate_element =
        convert_process_usage_to_element(
            "Highest aggregate RAM usage process",
            highest_ram_aggregate
        );
    viewmodel->push_back(move(highest_cpu_aggregate_element));
    viewmodel->push_back(move(highest_ram_aggregate_element));

    ProcessUsageInfo highest_cpu_toplevel =
        usage_reporter->get_procinfo_for_highest_toplevel_cpu_usage();
    ProcessUsageInfo highest_ram_toplevel =
        usage_reporter->get_procinfo_for_highest_aggregate_ram_usage();
    unique_ptr<ViewModelInternalNode> highest_cpu_toplevel_element =
        convert_process_usage_to_element(
            "Highest top-level CPU usage process",
            highest_cpu_toplevel
        );
    unique_ptr<ViewModelInternalNode> highest_ram_toplevel_element =
        convert_process_usage_to_element(
            "Highest top-level RAM usage process",
            highest_ram_toplevel
        );
    viewmodel->push_back(move(highest_cpu_toplevel_element));
    viewmodel->push_back(move(highest_ram_toplevel_element));

    return move(viewmodel);
}
