#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "cpu_usage.hpp"
#include "../windows_pdh_wrapper/PdhException.hpp"
#include "../windows_pdh_wrapper/pdh_wrapper.hpp"
#include "../utilities/windows_encoding_methods.hpp"

using batarim::encoding_methods::convert_wstring_to_string;
using std::cerr;
using std::clog;
using std::endl;
using std::pair;
using std::string;
using std::stringstream;
using std::vector;
using std::wstring;

namespace {

    string get_human_readable_name(string wmi_name)
    {
        int num;
        stringstream wss;
        wss << wmi_name;
    
        bool is_number = wss >> num != 0;
        if(is_number) {
            stringstream wss2;
            wss2 << ++num;
        
            string ret;
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
        vector<pair<string, int> >& usage_percentages
    ) {
        for(DWORD i = 0; i < num_of_processor_entries; ++i) {
            wstring wname = processor_utilizations[i].szName;
            string name = convert_wstring_to_string(wname);
            pair<string, int> processor_utilization(
                get_human_readable_name(name),
                (int)processor_utilizations[i].FmtValue.largeValue
            );
            usage_percentages.push_back(processor_utilization);
        }
    }

}

void get_cpu_usage(vector<pair<string, int> >& usage_percentages)
{
    PDH_HQUERY query = NULL;
    PDH_FMT_COUNTERVALUE_ITEM* processor_utilizations = NULL;
    try {
        const string context = "retrieve processor utilization";
        WCHAR* counter_path = L"\\Processor(*)\\% Processor Time";
        DWORD number_of_processor_entries;
        query_pdh(context, query, counter_path, processor_utilizations,
            number_of_processor_entries);

        fill_usage_percentages(processor_utilizations, 
            number_of_processor_entries, usage_percentages);
        
        cleanup(query, processor_utilizations);
    } catch(PdhException& e) {
        cerr << e.what() << endl;
        clog << e.what() << endl;
        cleanup(query, processor_utilizations);
    }
}
