#include <unistd.h>
#include <X11/Xlib.h>

#include <cstring>
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
using std::unique_ptr;

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
    
    string binary_name = get_filename_from_path_(process_name_);
    return binary_name;
}

string LinuxActiveWindow::get_filename_from_path_(string path)
{
    const char* const_c_path = path.c_str();
    unique_ptr<char> c_path(new char[strlen(const_c_path)+1]);
    strcpy(c_path.get(), const_c_path);
    char* token = strtok(c_path.get(), "/");
    char* last_token;
    while(token) {
        last_token = token;
        token = strtok(NULL, "/");
    }
    return last_token;
}
