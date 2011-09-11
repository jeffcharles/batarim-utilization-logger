#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <dirent.h>
#include <sys/types.h>

#include "LinuxRawProcessCollection.hpp"

using std::ifstream;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::vector;

shared_ptr<vector<unsigned int>> LinuxRawProcessCollection::get_pids_() const
{
    shared_ptr<vector<unsigned int>> pids(new vector<unsigned int>);
    
    DIR* proc_dir;
    struct dirent* dir_entry;
    proc_dir = opendir("/proc");
    while((dir_entry = readdir(proc_dir)) != NULL) {
        int pid;
        bool entry_is_directory = dir_entry->d_type == DT_DIR;
        if(entry_is_directory) {
            stringstream ss;
            ss << dir_entry->d_name;
            bool is_pid_entry = ss >> pid;
            if(is_pid_entry) {
                pids->push_back(pid);
            }
        }
    }
    closedir(proc_dir);
    return pids;
}

unsigned long long
LinuxRawProcessCollection::get_process_time_(unsigned int pid) const
{
    stringstream stat_file_path;
    stat_file_path << "/proc/" << pid << "/stat";
    ifstream proc_stat_file;
    proc_stat_file.open(stat_file_path.str());

    const int num_to_processor_times = 13;
    string trash;
    for(int i = 0; i < num_to_processor_times; ++i) {
        proc_stat_file >> trash;
    }

    unsigned long long cpu_time = 0;
    for(int i = 0; i < 2; ++i) {
        unsigned long temp;
        proc_stat_file >> temp;
        cpu_time += temp;
    }

    proc_stat_file.close();
    return cpu_time;
}

unsigned long long
LinuxRawProcessCollection::get_process_ram_usage_(unsigned int pid) const
{
    stringstream filepath;
    filepath << "/proc/" << pid << "/status";
    ifstream mem_status;
    mem_status.open(filepath.str());

    string field;
    // need to check that stream is still good since some status files do not
    // list a VmRSS field
    while(mem_status && field != "VmRSS:") {
        mem_status >> field;
    }
    
    unsigned long long ram_usage;
    if(!mem_status) {
        ram_usage = 0;
    } else {
        mem_status >> ram_usage;
    }
    
    mem_status.close();
    return ram_usage;
}
