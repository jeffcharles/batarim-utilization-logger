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

using std::cerr;
using std::endl;
using std::hex;
using std::pair;
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

void get_cpu_usage(vector<pair<wstring, int> >& usage_percentages)
{
	PDH_HQUERY query = NULL;
    PDH_STATUS pdh_status = PdhOpenQuery(
		NULL, // name of log file (NULL = use real-time data)
		NULL, // user-defined value to associate with query
		&query // handle to query
	);
	if(pdh_status != ERROR_SUCCESS) {
		cerr << "PdhOpenQuery failed when trying to retreive processor "
			<< "utilization with error 0x" << hex << pdh_status << endl;
		return;
	}

	PDH_HCOUNTER counter;
	pdh_status = PdhAddEnglishCounter(
		query, // handle to query
		"\\Processor(*)\\% Processor Time", // counter path
		NULL, // user-defined value (part of counter information)
		&counter // handle to counter
	);
	if(pdh_status != ERROR_SUCCESS) {
		cerr << "PdhAddCounter failed when trying to retrieve processor utilization "
			<< "with error 0x" << hex << pdh_status << endl;
		PdhCloseQuery(query);
		return;
	}

	pdh_status = PdhCollectQueryData(query);
	if(pdh_status != ERROR_SUCCESS) {
		cerr << "PdhCollectQueryData failed when trying to retrieve processor "
			<< "utilization with error 0x" << hex << pdh_status << endl;
		PdhCloseQuery(query);
		return;
	}

	Sleep(1000); // need to wait at least one second before querying again

	// query again to get second data point 
	// (see http://msdn.microsoft.com/en-us/library/aa371897(v=vs.85).aspx)
	pdh_status = PdhCollectQueryData(query);
	if(pdh_status != ERROR_SUCCESS) {
		cerr << "PdhCollectQueryData failed when trying to retrieve processor "
			<< "utilization with error 0x" << hex << pdh_status << endl;
		PdhCloseQuery(query);
		return;
	}

	DWORD buffer_size = 0;
	DWORD buffer_count;
	PDH_FMT_COUNTERVALUE_ITEM empty_items;
	pdh_status = PdhGetFormattedCounterArray(
		counter, // handle to counter
		PDH_FMT_LARGE, // format as 64-bit integers
		&buffer_size, // required size of buffer
		&buffer_count, // number of items returned
		&empty_items // result buffer
	);
	if(pdh_status != PDH_MORE_DATA) {
		cerr << "PdhGetFormattedCounterArray failed when trying to ascertain "
			<< "number of processors when trying to retrieve processor utilization "
			<< "with error 0x" << hex << pdh_status << endl;
		PdhCloseQuery(query);
		return;
	}
	
	auto processor_utilizations = new PDH_FMT_COUNTERVALUE_ITEM[buffer_size];
	pdh_status = PdhGetFormattedCounterArray(
		counter, // handle to counter
		PDH_FMT_LARGE, // format as 64-bit integers
		&buffer_size, // required size of buffer
		&buffer_count, // number of items returned
		processor_utilizations // result buffer
	);
	if(pdh_status != ERROR_SUCCESS) {
		cerr << "PdhGetFormattedCounterArray failed when trying to retrieve "
			<< "processor utilization with error 0x" << hex << pdh_status << endl;
		PdhCloseQuery(query);
		return;
	}
	for(DWORD i = 0; i < buffer_count; ++i) {
		std::string sname = processor_utilizations[i].szName;
		wstring name;
		name.assign(sname.begin(), sname.end());
		pair<wstring, int> processor_utilization(
			get_human_readable_name(name),
			(int)processor_utilizations[i].FmtValue.largeValue
		);
		usage_percentages.push_back(processor_utilization);
	}
	delete processor_utilizations;

	PdhCloseQuery(query);
}
