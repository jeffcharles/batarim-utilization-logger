#include <iostream>
#include <sys/sysinfo.h>

#include "ram_usage.hpp"

using std::endl;
using std::cerr;
using std::clog;

int get_ram_usage()
{
    // TODO: get more accurate reading by subtracting cached memory
    // try reading and parsing entries from /proc/meminfo
    struct sysinfo meminfo;
    int result = sysinfo(&meminfo);
    if(result == 0) {
        unsigned long used_memory = 
            meminfo.totalram - meminfo.freeram - meminfo.bufferram;
        int ram_usage_percentage = 
            (double)used_memory / meminfo.totalram * 100;
        return ram_usage_percentage;
    } else {
        cerr << "Error retrieving ram usage" << endl;
        clog << "Error retrieving ram usage" << endl;
        return -1;
    }
}
