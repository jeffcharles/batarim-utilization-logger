#ifndef GUARD_pdh_wrapper_h
#define GUARD_pdh_wrapper_h

#include <Windows.h>
#include <Pdh.h>

#include <string>

// WARNING: may throw pdh_exception
void query_pdh(
    const std::string& context,
    PDH_HQUERY& query,
    WCHAR* counter_path,
    PDH_FMT_COUNTERVALUE_ITEM*& result_set,
    DWORD& num_items_returned
);

void cleanup(PDH_HQUERY& query, PDH_FMT_COUNTERVALUE_ITEM*& result_set);

#endif