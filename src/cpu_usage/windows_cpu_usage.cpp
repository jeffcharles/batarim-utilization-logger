#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "cpu_usage.hpp"
#include "../windows_pdh_wrapper/pdh_exception.hpp"
#include "../windows_pdh_wrapper/pdh_wrapper.hpp"

using std::cerr;
using std::clog;
using std::endl;
using std::pair;
using std::string;
using std::vector;
using std::wstring;
using std::wstringstream;

wstring get_human_readable_name(wstring wmi_name)
{
    int num;
    wstringstream wss;
    wss << wmi_name;
    
    bool is_number = wss >> num != 0;
    if(is_number) {
        wstringstream wss2;
        wss2 << ++num;
        
        wstring ret;
        wss2 >> ret;
        return ret;
    }
    
    if(wmi_name[0] == '_') {
        return wmi_name.substr(1);
    } else {
        return wmi_name;
    }
}

void fill_usage_percentages(
    PDH_FMT_COUNTERVALUE_ITEM* processor_utilizations,
    DWORD num_of_processor_entries,
    vector<pair<wstring, int> >& usage_percentages
)
{
    for(DWORD i = 0; i < num_of_processor_entries; ++i) {
        string sname = processor_utilizations[i].szName;
        wstring name;
        name.assign(sname.begin(), sname.end());
        pair<wstring, int> processor_utilization(
            get_human_readable_name(name),
            (int)processor_utilizations[i].FmtValue.largeValue
        );
        usage_percentages.push_back(processor_utilization);
    }
}

void get_cpu_usage(vector<pair<wstring, int> >& usage_percentages)
{
    PDH_HQUERY query = NULL;
    PDH_FMT_COUNTERVALUE_ITEM* processor_utilizations = NULL;
    try {
        const string context = "retrieve processor utilization";

        populate_query(context, query);

        PDH_HCOUNTER counter = NULL;
        add_counter_to_query(context, query, counter, 
            "\\Processor(*)\\% Processor Time");

        collect_query_data(context, query);

        DWORD buffer_size = get_required_buffer_size(context, counter);
        processor_utilizations = 
            (PDH_FMT_COUNTERVALUE_ITEM*)malloc(buffer_size);
        DWORD number_of_processor_entries;
        populate_result_set(context, counter, buffer_size, 
            processor_utilizations, number_of_processor_entries);
        fill_usage_percentages(processor_utilizations, 
            number_of_processor_entries, usage_percentages);
        
        cleanup(query, processor_utilizations);
    } catch(pdh_exception& e) {
        cerr << e.what() << endl;
        clog << e.what() << endl;
        cleanup(query, processor_utilizations);
    }
}
