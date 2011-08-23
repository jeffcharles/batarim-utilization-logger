#include <X11/Xlib.h>

#include <fstream>
#include <sstream>
#include <string>

#include "LinuxActiveWindow.hpp"

using std::ifstream;
using std::string;
using std::stringstream;
using std::wstring;

wstring LinuxActiveWindow::get_name()
{
    if(name_ != L"") {
        return name_;
    }
    
    char* window_name = NULL;
    if(!XFetchName(display_.get(), window_, &window_name)) {
        if(window_name != NULL) {
            XFree(window_name);
        }
        return L"";
    }
    
    if(window_name == NULL) {
        return L"";
    }
    
    string s_window_name(window_name);
    XFree(window_name);
    name_.assign(s_window_name.begin(), s_window_name.end());

    return name_;
}

wstring LinuxActiveWindow::get_process_name()
{
    if(process_name_ != L"") {
        return process_name_;
    }

    stringstream filepath_ss;
    filepath_ss << "/proc/" << pid_ << "/cmdline";
    string filepath = filepath_ss.str();
    
    ifstream process_cmdline;
    process_cmdline.open(filepath.c_str());
    
    string process_name;
    process_cmdline >> process_name;
    
    process_cmdline.close();
    
    process_name_.assign(process_name.begin(), process_name.end());
    return process_name_;
}

int LinuxActiveWindow::get_cpu_usage()
{
    return -1;
}
