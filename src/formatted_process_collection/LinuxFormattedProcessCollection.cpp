#include <fstream>
#include <sstream>
#include <string>

#include "LinuxFormattedProcessCollection.hpp"

using std::ifstream;
using std::string;
using std::stringstream;

unsigned long long
LinuxFormattedProcessCollection::get_current_system_time_() const
{
    ifstream stat_file;
    stat_file.open("/proc/stat");

    string stat_line;
    getline(stat_file, stat_line);
    stringstream stat_stream(stat_line);

    stat_file.close();

    string trash;
    stat_stream >> trash;
    unsigned long long total_time = 0;
    while(stat_stream) {
        unsigned long temp;
        stat_stream >> temp;
        total_time += temp;
    }
    return total_time;
}

unsigned long long
LinuxFormattedProcessCollection::get_total_physical_ram_() const
{
    // /proc/meminfo has operating system memory usage information. The entry
    // we want is MemTotal which is the very first entry.

    ifstream meminfo_file;
    meminfo_file.open("/proc/meminfo");
                    
    // swallow "MemTotal:"
    string trash;
    meminfo_file >> trash;
                                    
    unsigned long long total_physical_ram;
    meminfo_file >> total_physical_ram;

    meminfo_file.close();

    return total_physical_ram;
}
