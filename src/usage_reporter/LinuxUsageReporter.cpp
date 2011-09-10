#include <fstream>
#include <ios>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "../utilities/utilities.hpp"

#include "LinuxUsageReporter.hpp"

using batarim::get_process_name;
using std::cerr;
using std::clog;
using std::endl;
using std::ifstream;
using std::istream;
using std::ostream;
using std::pair;
using std::shared_ptr;
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
