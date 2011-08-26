#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "cpu_usage/cpu_usage.hpp"
#include "ram_usage/ram_usage.hpp"
#include "active_window/active_window_module.hpp"
#include "active_window/IActiveWindow.hpp"

using std::cout;
using std::endl;
using std::pair;
using std::shared_ptr;
using std::string;
using std::vector;

int main()
{
    vector<pair<string, int> > cpu_usage_percentages;
    get_cpu_usage(cpu_usage_percentages);
    
    int ram_usage = get_ram_usage();

    shared_ptr<IActiveWindow> active_window = get_active_window();
    string active_window_name = active_window->get_name();
    string active_window_process_name = active_window->get_process_name();
    int active_window_cpu_usage = active_window->get_cpu_usage();
    
    typedef vector<pair<string, int> >::const_iterator ConstIterator;
    for(ConstIterator iter = cpu_usage_percentages.begin();
        iter != cpu_usage_percentages.end(); iter++) {
        
        cout << "CPU " << iter->first << " usage: " << iter->second << "%" 
            << endl;
    }
    cout << "Current RAM usage: " << ram_usage << "%" << endl;
    cout << "Active window: " << active_window_name << endl;
    cout << "Active window process: " << active_window_process_name << endl;
    cout << "Active window CPU usage: " << active_window_cpu_usage << "%"
        << endl;
}
