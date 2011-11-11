#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <dirent.h>
#include <sys/types.h>

#include "../utilities/utilities.hpp"

#include "LinuxRawProcessCollection.hpp"

using std::ifstream;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::vector;

unique_ptr<vector<unsigned int>> LinuxRawProcessCollection::get_pids_() const
{
    unique_ptr<vector<unsigned int>> pids(new vector<unsigned int>);
    
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

void LinuxRawProcessCollection::platform_specific_update_()
{
    unique_ptr<vector<unsigned int>> pids = get_pids_();
    for(vector<unsigned int>::const_iterator iter = pids->begin();
        iter != pids->end(); ++iter) {

        ProcessInformation& info = processes_[*iter];
        info.name = batarim::get_process_name(*iter);
        info.after_time = get_process_time_(*iter);
        info.ram_usage = get_process_ram_usage_(*iter);
        info.ppid = get_process_ppid_(*iter);
        
        set_last_update_run_(&info, current_update_run_);
    }
}

unsigned int
LinuxRawProcessCollection::get_process_ppid_(unsigned int pid) const
{
    stringstream filepath;
    filepath << "/proc/" << pid << "/stat";
    
    ifstream stat_file;
    stat_file.open(filepath.str());

    string field;
    while(stat_file && field != "PPid:") {
        stat_file >> field;
    }
    
    unsigned int ppid;
    if(stat_file) {
        stat_file >> ppid;
    } else {
        ppid = 0;
    }

    stat_file.close();
    return ppid;
}
