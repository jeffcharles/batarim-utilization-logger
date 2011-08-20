#ifndef GUARD_pdh_wrapper_h
#define GUARD_pdh_wrapper_h

#include <Windows.h>
#include <Pdh.h>

#include <string>

// NOTE: all functions below can potentially throw pdh_exception except cleanup

void populate_query(const std::string& context, PDH_HQUERY& query);

void add_counter_to_query(
    const std::string& context,
    PDH_HQUERY& query,
    PDH_HCOUNTER& counter,
    char* counter_path
);

void collect_query_data(const std::string& context, PDH_HQUERY& query);

DWORD get_required_buffer_size(
    const std::string& context,
    PDH_HCOUNTER& counter
);

void populate_result_set(
    const std::string& context,
    PDH_HCOUNTER& counter,
    DWORD buffer_size, 
    PDH_FMT_COUNTERVALUE_ITEM*& result_set,
    DWORD& num_items_returned
);

void cleanup(PDH_HQUERY& query, PDH_FMT_COUNTERVALUE_ITEM*& result_set);

#endif