#include <unistd.h>
#include <X11/Xlib.h>

#include <cstring>
#include <fstream>
#include <ios>
#include <memory>
#include <sstream>
#include <string>

#include "../utilities/utilities.hpp"

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
    process_name_ = batarim::get_process_name(pid_);
    return process_name_;
}
