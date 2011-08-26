#include <unistd.h>
#include <X11/Xlib.h>

#include <fstream>
#include <ios>
#include <memory>
#include <sstream>
#include <string>

#include "LinuxActiveWindow.hpp"

using std::ifstream;
using std::istream;
using std::iostream;
using std::shared_ptr;
using std::string;
using std::stringstream;

string LinuxActiveWindow::get_name()
{
    if(name_ != "") {
        return name_;
    }
    
    char* window_name = NULL;
    if(!XFetchName(display_.get(), window_, &window_name)) {
        if(window_name != NULL) {
            XFree(window_name);
        }
        return "";
    }
    
    if(window_name == NULL) {
        return "";
    }
    
    name_ = window_name;
    XFree(window_name);
    
    return name_;
}

string LinuxActiveWindow::get_process_name()
{
    if(process_name_ != "") {
        return process_name_;
    }

    stringstream filepath_ss;
    filepath_ss << "/proc/" << pid_ << "/cmdline";
    string filepath = filepath_ss.str();
    
    ifstream process_cmdline;
    process_cmdline.open(filepath.c_str());
    
    process_cmdline >> process_name_;
    
    process_cmdline.close();
    
    return process_name_;
}

int LinuxActiveWindow::get_cpu_usage()
{
    shared_ptr<istream> system_stat_stream1 = get_system_stat_line();
    unsigned long system_cpu_time1 = get_system_time(*system_stat_stream1);
    shared_ptr<istream> stat_stream1 = get_stat_stream();
    unsigned long process_cpu_time1 = get_process_cpu_time(*stat_stream1);
    
    sleep(1);

    shared_ptr<istream> system_stat_stream2 = get_system_stat_line();
    unsigned long system_cpu_time2 = get_system_time(*system_stat_stream2);
    shared_ptr<istream> stat_stream2 = get_stat_stream();
    unsigned long process_cpu_time2 = get_process_cpu_time(*stat_stream2);

    unsigned long system_time_diff = system_cpu_time2 - system_cpu_time1;
    unsigned long process_time_diff = process_cpu_time2 - process_cpu_time1;

    int cpu_usage = (double)process_time_diff / system_time_diff * 100;

    return cpu_usage;
}

shared_ptr<istream> LinuxActiveWindow::get_stat_stream()
{
    stringstream stat_file_path;
    stat_file_path << "/proc/" << pid_ << "/stat";
    
    ifstream stat_file;
    stat_file.open(stat_file_path.str());

    shared_ptr<iostream> stat_stream(new stringstream());
    *stat_stream << stat_file.rdbuf();

    stat_file.close();
    return stat_stream;
}

unsigned long LinuxActiveWindow::get_process_cpu_time(istream& stat_stream)
{
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

shared_ptr<istream> LinuxActiveWindow::get_system_stat_line()
{
    ifstream stat_file;
    stat_file.open("/proc/stat");

    string stat_line;
    getline(stat_file, stat_line);
    
    stat_file.close();
    shared_ptr<iostream> stat_stream(new stringstream(stat_line));

    return stat_stream;
}

unsigned long LinuxActiveWindow::get_system_time(istream& stat_line)
{
    string trash;
    stat_line >> trash;
    unsigned long long total_time = 0;
    while(stat_line) {
        unsigned long temp;
        stat_line >> temp;
        total_time += temp;
    }
    return total_time;
}
