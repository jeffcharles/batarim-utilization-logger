#include <fstream>
#include <functional>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <dirent.h>
#include <sys/types.h>

#include "LinuxUsageReporter.hpp"

using std::cerr;
using std::clog;
using std::endl;
using std::function;
using std::ifstream;
using std::istream;
using std::ostream;
using std::pair;
using std::pair;
using std::string;
using std::stringstream;
using std::vector;

namespace {
    class cpu_times
    {
        friend cpu_times operator-(const cpu_times&, const cpu_times&);

        private:
            string identifier_;
            unsigned long totaltime_;
            unsigned int idletime_;

            cpu_times(
                string identifier,
                unsigned long totaltime,
                unsigned int idletime
            ): 
                identifier_(identifier),
                totaltime_(totaltime),
                idletime_(idletime) { }
        public:
            cpu_times(istream& stat_stream_line)
            {
                // each CPU line in /proc/stat starts with an identifier (cpu or
                // cpu followed by a number), followed by usermode time, low
                // priority usermode time, system time, idle time, and then some
                // other times as detailed in the proc manpages, we just need
                // the identifier, the idle time, and the sum of all times
                stat_stream_line >> identifier_;

                totaltime_ = 0;
                unsigned long temp;
                const int num_entries_until_idle = 3;
                for(int i = 0; i < num_entries_until_idle; ++i) {
                    stat_stream_line >> temp;
                    totaltime_ += temp;
                }
                stat_stream_line >> temp;
                totaltime_ += temp;
                idletime_ = temp;
                while(stat_stream_line) {
                    stat_stream_line >> temp;
                    totaltime_ += temp;
                }
            }

            string get_identifier() { return identifier_; }
            unsigned long get_usedtime() { return totaltime_ - idletime_; }
            unsigned long get_totaltime() { return totaltime_; }
    };

    cpu_times operator-(const cpu_times& lhs, const cpu_times& rhs)
    {
        string identifier = lhs.identifier_;
        unsigned long totaltime = lhs.totaltime_ - rhs.totaltime_;
        unsigned int idletime = lhs.idletime_ - rhs.idletime_;
        return cpu_times(identifier, totaltime, idletime);
    }
}

bool LinuxUsageReporter::populate_processors_usage_(stringstream& before_stream)
{
    stringstream after_stream;
    populate_stat_stream_(after_stream);

    for(;;) {
        string stat_line1, stat_line2;
        getline(before_stream, stat_line1);
        getline(after_stream, stat_line2);

        bool is_cpu_line = stat_line1.substr(0, 3) == "cpu";
        if(!is_cpu_line) {
            break;
        }

        stringstream stat_line_stream1(stat_line1);
        stringstream stat_line_stream2(stat_line2);
        cpu_times times1(stat_line_stream1);
        cpu_times times2(stat_line_stream2);
        cpu_times difference = times2 - times1;

        int utilization = (double)(difference.get_usedtime()) /
            difference.get_totaltime() * 100 + 0.5;

        string raw_identifier = difference.get_identifier();
        string cpu_identifier = get_human_readable_name_for_processor_entry_(
            raw_identifier);
        pair<string, int> usage_percentage(
            cpu_identifier, utilization);
        processor_usages_->push_back(usage_percentage);
    }
    
    return true;
}

unsigned long long LinuxUsageReporter::get_current_system_time_()
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

bool LinuxUsageReporter::populate_process_list_(
    function<void(ProcessList&, unsigned int, unsigned long long)> set_time
) {
    DIR* proc_dir;
    struct dirent* proc_dir_entry;
    proc_dir = opendir("/proc");
    while((proc_dir_entry = readdir(proc_dir)) != NULL) {
        int pid;
        if(!dir_entry_is_pid_(proc_dir_entry, &pid)) {
            continue;
        }

        stringstream stat_file_path;
        stat_file_path << "/proc/" << pid << "/stat";
        ifstream proc_stat_file;
        proc_stat_file.open(stat_file_path.str());
        stringstream proc_stat_stream;
        proc_stat_stream << proc_stat_file.rdbuf();
        proc_stat_file.close();

        unsigned long long cpu_time = get_process_cpu_time_(proc_stat_stream);
        set_time(process_list_, pid, cpu_time); 
    }
    closedir(proc_dir);
    return true;
}

string LinuxUsageReporter::get_human_readable_name_for_processor_entry_(
    string& provided_name
) {
    // CPU lines in /proc/stat start with either cpu (indicating average cpu
    // usage) or with cpu followed by a number starting from 0 indicating which
    // processor core the measurement is for, the identifier shown to the user
    // should be "Total" for the average utilization across processor cores or
    // just the core number (without the cpu in front) but cores should be
    // 1-indexed not zero indexed so we need to increment that number by one
    if(provided_name.length() > 3) {
        string s_identifier_number = provided_name.substr(3);
        stringstream ss;
        ss << s_identifier_number;
        int i_identifier_number;
        ss >> i_identifier_number;
        stringstream ss2;
        ss2 << ++i_identifier_number;
        return ss2.str();
    } else {
        return "Total";
    }
}

void LinuxUsageReporter::populate_stat_stream_(ostream& stat_stream)
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

// Checks if dir_entry represents a pid and populates pid if it is
bool LinuxUsageReporter::dir_entry_is_pid_(struct dirent* dir_entry, int* pid)
{
    if(dir_entry->d_type != DT_DIR) {
        return false;
    }
    stringstream ss;
    ss << dir_entry->d_name;
    bool is_pid = ss >> *pid;
    return is_pid;
}

unsigned long long LinuxUsageReporter::get_process_cpu_time_(
    istream& stat_stream
) {
    const int num_to_processor_times = 13;
    string trash;
    for(int i = 0; i < num_to_processor_times; ++i) {
        stat_stream >> trash;
    }

    unsigned long long cpu_time = 0;
    for(int i = 0; i < 2; ++i) {
        unsigned long temp;
        stat_stream >> temp;
        cpu_time += temp;
    }

    return cpu_time;
}

