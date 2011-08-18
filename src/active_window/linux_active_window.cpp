#include <X11/Xlib.h>

#include <string>

#include "active_window.hpp"

using std::string;
using std::wstring;

wstring get_active_window_name()
{
    Display* display = XOpenDisplay(NULL);
    if(display == NULL) {
        return L"No local active window";
    }
    
    Window focus_return;
    int revert_to_return;
    XGetInputFocus(display, &focus_return, &revert_to_return);
    if(focus_return == 0) {
        return L"No local active window";
    }
    if(focus_return == DefaultRootWindow(display)) {
        return L"Desktop";
    }

    // in Gnome, need to get parent window to get actual window title
    // TODO: figure out if KDE, XFCE work similarly
    Window root;
    Window parent;
    Window* children;
    unsigned int num_children;
    if(!XQueryTree(display, focus_return, &root, &parent, &children, 
        &num_children)) {
        
        return L"No local active window";
    }
    XFree(children);

    char* window_name = NULL;
    if(!XFetchName(display, parent, &window_name)) {
        if(window_name != NULL) {
            XFree(window_name);
        }
        return L"No local active window";
    }
    
    if(window_name == NULL) {
        return L"";
    }
    string s_window_name(window_name);
    XFree(window_name);
    wstring ret;
    ret.assign(s_window_name.begin(), s_window_name.end());
    return ret;
}

wstring get_active_window_process_name()
{
    return L"Not implmented";
}
