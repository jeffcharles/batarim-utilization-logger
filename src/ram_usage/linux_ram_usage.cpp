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
using std::iostream;
using std::istream;
using std::map;
using std::ostream;
using std::streamsize;
using std::string;
using std::stringstream;

void get_meminfo_stream(ostream& meminfo_stream)
{
    ifstream meminfo_file;
    meminfo_file.open("/proc/meminfo");
    if(!meminfo_file) {
        cerr << "Could not open /proc/meminfo to retrieve ram usage" << endl;
        clog << "Could not open /proc/meminfo to retrieve ram usage" << endl;
    }

    meminfo_stream << meminfo_file.rdbuf();
    meminfo_file.close();
}

map<string, unsigned long> get_meminfo_from_stream(istream& meminfo_stream)
{
    map<string, unsigned long> meminfo;
    
    streamsize line_buffer_size = 81;
    char line_buffer[line_buffer_size];
    while(meminfo_stream.getline(line_buffer, line_buffer_size)) {
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
    return meminfo;
}

int get_used_ram(map<string, unsigned long>& meminfo)
{
    unsigned long totalram = meminfo["MemTotal"];
    unsigned long freeram = meminfo["MemFree"];
    unsigned long bufferram = meminfo["Buffers"];
    unsigned long cacheram = meminfo["Cached"];

    unsigned long used_ram = totalram - freeram - bufferram - cacheram;
    return (double)used_ram / totalram * 100;
}

int get_ram_usage()
{
    stringstream meminfo_stream;
    get_meminfo_stream(meminfo_stream);
    map<string, unsigned long> meminfo = 
        get_meminfo_from_stream(meminfo_stream);
    return get_used_ram(meminfo);
}
