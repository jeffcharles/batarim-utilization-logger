#include <memory>
#include <sstream>
#include <string>
#include <vector>

// Needed for using PDH
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
// --------------------
#include <Psapi.h>

#include "../utilities/utilities.hpp"
#include "../utilities/windows_utilities.hpp"

#include "WindowsUsageReporter.hpp"

using batarim::convert_wstring_to_string;
using std::pair;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::vector;
using std::wstring;

bool WindowsUsageReporter::initial_cpu_sweep_(PdhData& pdh_data)
{
    PDH_STATUS pdh_status = PdhOpenQuery(
        NULL, // name of log file (NULL = use real-time data)
        NULL, // user-defined value to associate with query
        &(pdh_data.query) // handle to query
    );
    if(pdh_status != ERROR_SUCCESS) {
        return false;
    }
    
    pdh_status = PdhAddEnglishCounter(
        pdh_data.query, // handle to query
        L"\\Processor(*)\\% Processor Time", // counter path
        NULL, // user-defined value (part of counter information)
        &(pdh_data.counter) // handle to counter
    );
    if(pdh_status != ERROR_SUCCESS) {
        return false;
    }

    pdh_status = PdhCollectQueryData(pdh_data.query);
    if(pdh_status != ERROR_SUCCESS) {
        return false;
    }
    
    return true;
}

bool WindowsUsageReporter::populate_processors_usage_(
    PdhData& pdh_data
) {
    PDH_STATUS pdh_status = PdhCollectQueryData(pdh_data.query);
    if(pdh_status != ERROR_SUCCESS) {
        return false;
    }

    DWORD num_bytes_required = 0;
    DWORD buffer_count;
    PDH_FMT_COUNTERVALUE_ITEM empty_items;
    pdh_status = PdhGetFormattedCounterArray(
        pdh_data.counter, // handle to counter
        PDH_FMT_LARGE, // format as 64-bit integers
        &num_bytes_required, // required size of buffer
        &buffer_count, // number of items returned
        &empty_items // result buffer
    );
    if(pdh_status != PDH_MORE_DATA) {
        return false;
    }

    unique_ptr<PDH_FMT_COUNTERVALUE_ITEM> processor_utilizations(
        (PDH_FMT_COUNTERVALUE_ITEM*)malloc(num_bytes_required));

    DWORD num_of_processor_entries;
    pdh_status = PdhGetFormattedCounterArray(
        pdh_data.counter, // handle to counter
        PDH_FMT_LARGE, // format as 64-bit integers
        &num_bytes_required, // required size of buffer
        &num_of_processor_entries, // number of items returned
        processor_utilizations.get() // result buffer
    );
    if(pdh_status != ERROR_SUCCESS) {
        return false;
    }
    
    for(DWORD i = 0; i < num_of_processor_entries; ++i) {
        wstring wname = processor_utilizations.get()[i].szName;
        string name = convert_wstring_to_string(wname);
        pair<string, int> processor_utilization(
            get_human_readable_name_for_processor_entry_(name),
            (int)processor_utilizations.get()[i].FmtValue.largeValue
        );
        processor_usages_.push_back(processor_utilization);
    }

    return true;
}

string WindowsUsageReporter::get_human_readable_name_for_processor_entry_(
    string& provided_name
) {
    int num;
    stringstream wss;
    wss << provided_name;
    
    bool is_number = wss >> num != 0;
    if(is_number) {
        stringstream wss2;
        wss2 << ++num;
        
        string ret;
        wss2 >> ret;
        return ret;
    }
    
    if(provided_name[0] == '_') {
        return provided_name.substr(1);
    } else {
        return provided_name;
    }
}