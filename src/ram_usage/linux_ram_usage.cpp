#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include <sys/sysinfo.h>

#include "ram_usage.hpp"

using std::endl;
using std::cerr;
using std::clog;
using std::ifstream;
using std::map;
using std::streamsize;
using std::string;
using std::stringstream;

int get_ram_usage()
{
    map<string, unsigned long> meminfo;
    stringstream meminfo_string;
    ifstream meminfo_file;
    
    // Copy from meminfo_file into meminfo_string
    meminfo_file.open("/proc/meminfo");
    if(!meminfo_file) {
        cerr << "Could not open /proc/meminfo to retrieve ram usage" << endl;
        clog << "Could not open /proc/meminfo to retrieve ram usage" << endl;
        return -1;
    }

    char c;
    while(meminfo_file.get(c)) {
        meminfo_string.put(c);
    }

    meminfo_file.close();

    // Parse from meminfo_string into meminfo
    streamsize line_buffer_size = 81;
    char line_buffer[line_buffer_size];
    while(meminfo_string.getline(line_buffer, line_buffer_size)) {
        string line(line_buffer);
        string::size_type colonpos = line.find(':');
        string varname = line.substr(0, colonpos);
        string::size_type first_digit = 0;
        string::size_type last_digit = 0;
        for(string::size_type i = colonpos; line[i] != '\0'; ++i) {
            if(isdigit(line[i]) && first_digit == 0) {
                first_digit = i;
            }
            if(isdigit(line[i]) && first_digit > 0) {
                last_digit = i;
            }
        }
        bool found_digits = first_digit > 0 && last_digit > 0;
        string varvalue_str = found_digits ?
            line.substr(first_digit, last_digit - first_digit) :
            "-1";
        stringstream varvalue_converter;
        varvalue_converter << varvalue_str;
        unsigned long varvalue;
        varvalue_converter >> varvalue;

        meminfo[varname] = varvalue;
    }

    // Calculate free memory by subtracting used memory from total
    unsigned long totalram = meminfo["MemTotal"];
    unsigned long freeram = meminfo["MemFree"];
    unsigned long bufferram = meminfo["Buffers"];
    unsigned long cacheram = meminfo["Cached"];

    unsigned long used_ram = totalram - freeram - bufferram - cacheram;
    return (double)used_ram / totalram * 100;
}
