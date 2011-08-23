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

namespace {

    class cpu_times
    {
        friend cpu_times operator-(const cpu_times&, const cpu_times&);

        private:
            wstring identifier;
            unsigned long totaltime;
            unsigned int idletime;

            cpu_times(
                wstring identifier,
                unsigned long totaltime,
                unsigned int idletime
            ): 
                identifier(identifier),
                totaltime(totaltime),
                idletime(idletime) { }
        public:
            cpu_times(istream& stat_stream_line)
            {
                // each CPU line in /proc/stat starts with an identifier (cpu or
                // cpu followed by a number), followed by usermode time, low
                // priority usermode time, system time, idle time, and then some
                // other times as detailed in the proc manpages, we just need
                // the identifier, the idle time, and the sum of all times
                string s_identifier;
                stat_stream_line >> s_identifier;
                identifier.assign(s_identifier.begin(), s_identifier.end());

                totaltime = 0;
                unsigned long temp;
                const int num_entries_until_idle = 3;
                for(int i = 0; i < num_entries_until_idle; ++i) {
                    stat_stream_line >> temp;
                    totaltime += temp;
                }
                stat_stream_line >> temp;
                totaltime += temp;
                idletime = temp;
                while(stat_stream_line) {
                    stat_stream_line >> temp;
                    totaltime += temp;
                }
            }

            wstring get_identifier()
            {
                // CPU lines in /proc/stat start with either cpu (indicating 
                // average cpu usage) or with cpu followed by a number starting 
                // from 0 indicating which processor core the measurement is
                // for, the identifier shown to the user should be "Total" for
                // the average utilization across processor cores or just the 
                // core number (without the cpu in front) but cores should be
                // 1-indexed not zero indexed so we need to increment that 
                // number by one
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

            unsigned long get_usedtime() { return totaltime - idletime; }
            unsigned long get_totaltime() { return totaltime; }
    };

    cpu_times operator-(const cpu_times& lhs, const cpu_times& rhs)
    {
        wstring identifier = lhs.identifier;
        unsigned long totaltime = lhs.totaltime - rhs.totaltime;
        unsigned int idletime = lhs.idletime - rhs.idletime;
        return cpu_times(identifier, totaltime, idletime);
    }

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
    
        pair<wstring, int> usage_percentage(
            difference.get_identifier(), utilization);
        usage_percentages.push_back(usage_percentage);
    }
}
