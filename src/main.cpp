#include <iostream>
#include <string>
#include <vector>
#include "cpu_usage/cpu_usage.hpp"
#include "ram_usage/ram_usage.hpp"
#include "active_window/ActiveWindow.hpp"

using std::endl;
using std::pair;
using std::vector;
using std::wcout;
using std::wstring;

int main()
{
    vector<pair<wstring, int> > cpu_usage_percentages;
    get_cpu_usage(cpu_usage_percentages);
    
    int ram_usage = get_ram_usage();

    ActiveWindow active_window;
    wstring active_window_name = active_window.get_name();
    wstring active_window_process_name = active_window.get_process_name();
    
    typedef vector<pair<wstring, int> >::const_iterator ConstIterator;
    for(ConstIterator iter = cpu_usage_percentages.begin();
        iter != cpu_usage_percentages.end(); iter++) {
        
        wcout << L"CPU " << iter->first << L" usage: " << iter->second << L"%" 
            << endl;
    }
    wcout << L"Current RAM usage: " << ram_usage << L"%" << endl;
    wcout << L"Active window: " << active_window_name << endl;
    wcout << L"Active window process: " << active_window_process_name << endl;
}
