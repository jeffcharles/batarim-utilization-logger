#include <fstream>

#include "uptime.hpp"

using std::ifstream;

unsigned int get_uptime()
{
    ifstream uptime_file;
    uptime_file.open("/proc/uptime");

    unsigned int uptime;
    uptime_file >> uptime;

    uptime_file.close();
    
    return uptime;
}
