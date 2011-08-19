#include <X11/Xlib.h>

#include <fstream>
#include <sstream>
#include <string>

#include "active_window.hpp"

using std::ifstream;
using std::string;
using std::stringstream;
using std::wstring;

void populate_display_and_window(Display*& display, Window& window)
{
    display = XOpenDisplay(NULL);
    if(display == NULL) {
        return;
    }
   
    Window focused_window;
    int revert_to_return;
    XGetInputFocus(display, &focused_window, &revert_to_return);
    if(focused_window == None) {
        return;
    }
    if(focused_window == DefaultRootWindow(display)) {
        return;
    }
    
    // in Gnome, need to get parent window to get actual window title
    // TODO: figure out if KDE, XFCE work similarly
    Window root;
    Window parent;
    Window* children;
    unsigned int num_children;
    if(!XQueryTree(display, focused_window, &root, &parent, &children, 
        &num_children)) {
        
        return;
    }
    XFree(children);
    
    window = parent;
}

wstring get_active_window_name()
{
    Display* display = NULL;
    Window window;
    populate_display_and_window(display, window);
    if(display == NULL || window == None) {
        return L"No local active window";
    }

    char* window_name = NULL;
    if(!XFetchName(display, window, &window_name)) {
        if(window_name != NULL) {
            XFree(window_name);
        }
        XFree(display);
        return L"No local active window";
    }
    XFree(display);
    
    if(window_name == NULL) {
        return L"";
    }
    string s_window_name(window_name);
    XFree(window_name);
    wstring ret;
    ret.assign(s_window_name.begin(), s_window_name.end());
    return ret;
}

string get_process_name(long pid)
{
    stringstream filepath_ss;
    filepath_ss << "/proc/" << pid << "/cmdline";
    string filepath = filepath_ss.str();
    
    ifstream process_cmdline;
    process_cmdline.open(filepath.c_str());
    
    string process_name;
    process_cmdline >> process_name;
    
    process_cmdline.close();
    
    return process_name;
}

wstring get_wide_process_name(long pid)
{
    string process_name = get_process_name(pid);
    wstring wprocess_name;
    wprocess_name.assign(process_name.begin(), process_name.end());
    return wprocess_name;
}

wstring get_active_window_process_name()
{    
    Display* display = NULL;
    Window window;
    populate_display_and_window(display, window);
    if(display == NULL || window == None) {
        return L"No local active window";
    }
    if(window == DefaultRootWindow(display)) {
        XFree(display);
        return L"Desktop";
    }

    Atom pid_property = XInternAtom(display, "_NET_WM_PID", true);
    if(pid_property == None) {
        XFree(display);
        return L"";
    }
    
    Atom actual_type;
    int actual_format;
    unsigned long num_items;
    unsigned long bytes_after;
    unsigned char* property;
    bool pid_available = XGetWindowProperty(
        display, 
        window, 
        pid_property, 
        0, // specifies offset in specified property
        1, // specifies length of data to retrieve
        false, // delete property?
        AnyPropertyType, // atom identifier associated with the property type
        &actual_type,
        &actual_format,
        &num_items,
        &bytes_after,
        &property
    ) == Success;
    XFree(display);

    bool valid_results = 
        pid_available && actual_type != None && 
        actual_format == 32 && num_items > 0;
    if(!valid_results) {
        return L"";
    }
    
    long pid = ((long*)property)[0];
    XFree(property);
    return get_wide_process_name(pid);
}
