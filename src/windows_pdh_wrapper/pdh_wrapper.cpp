// Needed for using PDH
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
// --------------------

#include "pdh_wrapper.hpp"
#include "PdhException.hpp"

using std::string;

namespace {

    void populate_query(const string& context, PDH_HQUERY& query)
    {
        PDH_STATUS pdh_status = PdhOpenQuery(
            NULL, // name of log file (NULL = use real-time data)
            NULL, // user-defined value to associate with query
            &query // handle to query
        );
        if(pdh_status != ERROR_SUCCESS) {
            throw PdhException("PdhOpenQuery", context, pdh_status);
        }
    }

    void add_counter_to_query(
        const string& context,
        PDH_HQUERY& query,
        PDH_HCOUNTER& counter,
        char* counter_path
    ) {
        PDH_STATUS pdh_status = PdhAddEnglishCounter(
            query, // handle to query
            counter_path, // counter path
            NULL, // user-defined value (part of counter information)
            &counter // handle to counter
        );
        if(pdh_status != ERROR_SUCCESS) {
            throw PdhException("PdhAddEnglishCounter", context, pdh_status);
        }
    }

    void collect_query_data(const string& context, PDH_HQUERY& query)
    {
        PDH_STATUS pdh_status = PdhCollectQueryData(query);
        if(pdh_status != ERROR_SUCCESS) {
            throw PdhException("PdhCollectQueryData", context, pdh_status);
        }

        Sleep(1000); // need to wait at least one second before querying again

        // query again to get second data point 
        // (see http://msdn.microsoft.com/en-us/library/aa371897(v=vs.85).aspx)
        pdh_status = PdhCollectQueryData(query);
        if(pdh_status != ERROR_SUCCESS) {
            throw PdhException("PdhCollectQueryData", context, pdh_status);
        }
    }

    DWORD get_required_buffer_size(
        const string& context,
        PDH_HCOUNTER& counter
    ) {
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
            throw PdhException("PdhGetFormattedCounterArray", context, 
                pdh_status);
        }
        return number_of_bytes_required;
    }

    void populate_result_set(
        const string& context,
        PDH_HCOUNTER& counter,
        DWORD buffer_size, 
        PDH_FMT_COUNTERVALUE_ITEM*& result_set,
        DWORD& num_items_returned
    ) {
        PDH_STATUS pdh_status = PdhGetFormattedCounterArray(
            counter, // handle to counter
            PDH_FMT_LARGE, // format as 64-bit integers
            &buffer_size, // required size of buffer
            &num_items_returned, // number of items returned
            result_set // result buffer
        );
        if(pdh_status != ERROR_SUCCESS) {
            throw PdhException("PdhGetFormattedCounterArray", context, 
                pdh_status);
        }
    }

}

void cleanup(PDH_HQUERY& query, PDH_FMT_COUNTERVALUE_ITEM*& result_set)
{
    if(query != NULL) {
        PdhCloseQuery(query);
    }
    if(result_set != NULL) {
        free(result_set);
        result_set = NULL;
    }
}

void query_pdh(
    const string& context,
    PDH_HQUERY& query,
    char* counter_path,
    PDH_FMT_COUNTERVALUE_ITEM*& result_set,
    DWORD& num_items_returned
) {
    populate_query(context, query);
    
    PDH_HCOUNTER counter = NULL;
    add_counter_to_query(context, query, counter, counter_path);
    
    collect_query_data(context, query);
    
    DWORD buffer_size = get_required_buffer_size(context, counter);
    result_set = (PDH_FMT_COUNTERVALUE_ITEM*)malloc(buffer_size);
    
    populate_result_set(context, counter, buffer_size, result_set,
        num_items_returned);
}