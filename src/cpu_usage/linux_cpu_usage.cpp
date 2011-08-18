#include <unistd.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "cpu_usage.hpp"

using std::cerr;
using std::clog;
using std::endl;
using std::ifstream;
using std::istream;
using std::ostream;
using std::pair;
using std::string;
using std::stringstream;
using std::vector;
using std::wstring;
using std::wstringstream;

struct cpu_times
{
    wstring identifier;
    unsigned int usermode;
    unsigned int lowpriority_usermode;
    unsigned int systemmode;
    unsigned int idletime;
};

void populate_stat_stream(ostream& stat_stream)
{
    ifstream stat_file;
    stat_file.open("/proc/stat");
    if(!stat_file) {
        cerr << "Could not open /proc/stat to get cpu utilizations" << endl;
        clog << "Could not open /proc/stat to get cpu utilizations" << endl;
        return;
    }
    stat_stream << stat_file.rdbuf();
    stat_file.close();
}

void populate_cpu_time_struct(istream& stat_stream, cpu_times& times)
{
    string identifier;
    stat_stream >> identifier;
    times.identifier.assign(identifier.begin(), identifier.end());
    stat_stream >> times.usermode;
    stat_stream >> times.lowpriority_usermode;
    stat_stream >> times.systemmode;
    stat_stream >> times.idletime;
}

void subtract_cpu_times(
    cpu_times& times1, 
    cpu_times& times2, 
    cpu_times& result
) {
    result.identifier = times1.identifier;
    result.usermode = times2.usermode - times1.usermode;
    result.lowpriority_usermode = 
        times2.lowpriority_usermode - times1.lowpriority_usermode;
    result.systemmode = times2.systemmode - times1.systemmode;
    result.idletime = times2.idletime - times1.idletime;
}

wstring format_identifier(wstring& identifier)
{
    if(identifier.length() > 3) {
        wstring ws_identifier_number = identifier.substr(3);
        wstringstream wss;
        wss << ws_identifier_number;
        int i_identifier_number;
        wss >> i_identifier_number;
        wstringstream wss2;
        wss2 << ++i_identifier_number;
        return wss2.str();
    } else {
        return L"Total";
    }
}

void get_cpu_usage(vector<pair<wstring, int> >& usage_percentages)
{
    stringstream stat_stream1;
    stringstream stat_stream2;
    populate_stat_stream(stat_stream1);
    sleep(1);
    populate_stat_stream(stat_stream2);

    for(;;) {
        string stat_line1, stat_line2;
        getline(stat_stream1, stat_line1);
        getline(stat_stream2, stat_line2);
        if(stat_line1.substr(0, 3) != "cpu") {
            break;
        }

        stringstream stat_line_stream1(stat_line1);
        stringstream stat_line_stream2(stat_line2);
        cpu_times times1, times2;
        populate_cpu_time_struct(stat_line_stream1, times1);
        populate_cpu_time_struct(stat_line_stream2, times2);

        cpu_times difference;
        subtract_cpu_times(times1, times2, difference);

        unsigned long total_time = 
            difference.usermode + difference.lowpriority_usermode + 
            difference.systemmode + difference.idletime;
        int utilization = 
            (double)(total_time - difference.idletime) / total_time * 100;
    
        wstring identifier = format_identifier(difference.identifier);

        pair<wstring, int> usage_percentage(identifier, utilization);
        usage_percentages.push_back(usage_percentage);
    }
}
