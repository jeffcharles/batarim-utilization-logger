// Needed for using PDH
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
// --------------------

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "cpu_usage.hpp"
#include "windows_pdh_exception.hpp"

using std::cerr;
using std::clog;
using std::endl;
using std::hex;
using std::pair;
using std::string;
using std::vector;
using std::wstring;
using std::wstringstream;

void get_query(PDH_HQUERY& query)
{
    PDH_STATUS pdh_status = PdhOpenQuery(
        NULL, // name of log file (NULL = use real-time data)
        NULL, // user-defined value to associate with query
        &query // handle to query
    );
    if(pdh_status != ERROR_SUCCESS) {
        throw pdh_exception("PdhOpenQuery failed when trying to retrive "
            "processor utilization.", pdh_status);
    }
}

void add_counter_to_query(PDH_HQUERY& query, PDH_HCOUNTER& counter)
{
    PDH_STATUS pdh_status = PdhAddEnglishCounter(
        query, // handle to query
        "\\Processor(*)\\% Processor Time", // counter path
        NULL, // user-defined value (part of counter information)
        &counter // handle to counter
    );
    if(pdh_status != ERROR_SUCCESS) {
        throw pdh_exception("PdhAddEnglishCounter failed when trying to "
            "retrieve processor utilization.", pdh_status);
    }
}

void collect_query_data(PDH_HQUERY& query)
{
    PDH_STATUS pdh_status = PdhCollectQueryData(query);
    if(pdh_status != ERROR_SUCCESS) {
        throw pdh_exception("PdhCollectQueryData failed when trying to "
            "retrieve processor utilization.", pdh_status);
    }

    Sleep(1000); // need to wait at least one second before querying again

    // query again to get second data point 
    // (see http://msdn.microsoft.com/en-us/library/aa371897(v=vs.85).aspx)
    pdh_status = PdhCollectQueryData(query);
    if(pdh_status != ERROR_SUCCESS) {
        throw pdh_exception("The second call to PdhCollectQueryData failed "
            "when trying to retrieve processor utilization.", pdh_status);
    }
}

DWORD get_required_buffer_size(PDH_HCOUNTER& counter)
{
    DWORD number_of_bytes_required = 0;
    DWORD buffer_count;
    PDH_FMT_COUNTERVALUE_ITEM empty_items;
    PDH_STATUS pdh_status = PdhGetFormattedCounterArray(
        counter, // handle to counter
        PDH_FMT_LARGE, // format as 64-bit integers
        &number_of_bytes_required, // required size of buffer
        &buffer_count, // number of items returned
        &empty_items // result buffer
    );
    if(pdh_status != PDH_MORE_DATA) {
        throw pdh_exception("PdhGetFormattedCounterArray failed when trying "
            "to ascertain the number of processor entries when trying to "
            "retrieve processor utilization", pdh_status);
    }
    return number_of_bytes_required;
}

void populate_processor_utilizations(
    PDH_HCOUNTER& counter,
    DWORD buffer_size, 
    PDH_FMT_COUNTERVALUE_ITEM*& processor_utilizations,
    DWORD& num_items_returned
)
{
    PDH_STATUS pdh_status = PdhGetFormattedCounterArray(
        counter, // handle to counter
        PDH_FMT_LARGE, // format as 64-bit integers
        &buffer_size, // required size of buffer
        &num_items_returned, // number of items returned
        processor_utilizations // result buffer
    );
    if(pdh_status != ERROR_SUCCESS) {
        throw pdh_exception("PdhGetFormattedCounterArray failed when trying "
            "to retrieve processor utilization", pdh_status);
    }
}

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

void cleanup(PDH_HQUERY& query, PDH_FMT_COUNTERVALUE_ITEM*& countervalues)
{
    if(query != NULL) {
        PdhCloseQuery(query);
    }
    if(countervalues != NULL) {
        free(countervalues);
        countervalues = NULL;
    }
}

void get_cpu_usage(vector<pair<wstring, int> >& usage_percentages)
{
    PDH_HQUERY query = NULL;
    PDH_FMT_COUNTERVALUE_ITEM* processor_utilizations = NULL;
    try {
        get_query(query);

        PDH_HCOUNTER counter = NULL;
        add_counter_to_query(query, counter);

        collect_query_data(query);

        DWORD buffer_size = get_required_buffer_size(counter);
        processor_utilizations = 
            (PDH_FMT_COUNTERVALUE_ITEM*)malloc(buffer_size);
        DWORD number_of_processor_entries;
        populate_processor_utilizations(counter, buffer_size, 
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
