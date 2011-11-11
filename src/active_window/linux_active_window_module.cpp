#include <memory>

#include <X11/Xlib.h>

#include "IActiveWindow.hpp"
#include "LinuxActiveWindow.hpp"
#include "LinuxInvalidActiveWindow.hpp"

#include "active_window_module.hpp"

using std::unique_ptr;

namespace {
    class XDeleter
    {
        public:
            template <typename T>
            void operator()(T* thing_to_delete) const
            {
                XFree(thing_to_delete);
            }
    };
}

unique_ptr<IActiveWindow> get_active_window()
{
    unique_ptr<IActiveWindow> invalid_window(new LinuxInvalidActiveWindow());

    // Explicitly providing display name instead of NULL because XOpenDisplay
    // cannot resolve the display name when running application from another
    // user account outside of the current X session
    // NOTE: the user account the application is running under needs to be
    // added to xhosts to open the display
    // NOTE: ":0" may not always be the correct display, but it should be on
    // the majority of single-user desktop systems
    unique_ptr<Display, XDeleter> display(XOpenDisplay(":0"));
    if(display.get() == NULL) {
        return invalid_window;
    }

    Window focused_window;
    int revert_to_return;
    XGetInputFocus(display.get(), &focused_window, &revert_to_return);
    if(focused_window == None || 
        focused_window == DefaultRootWindow(display.get())) {
        
        return invalid_window;
    }

    // in Gnome, need to get parent window to get actual window title
    // TODO: figure out if KDE, XFCE work similarly
    Window root;
    Window parent;
    Window* children = NULL;
    unsigned int num_children;
    if(!XQueryTree(display.get(), focused_window, &root, &parent,
        &children, &num_children)) {

        if(children != NULL) {
            XFree(children);
        }

        return invalid_window;
    }
    XFree(children);

    // try to find PID through xlib
    // WARNING: _NET_WM_PID is not part of the X11 standard, so this may not
    // work for all X11 window managers
    Atom pid_property = XInternAtom(display.get(), "_NET_WM_PID", true);
    if(pid_property == None) {
        return invalid_window;
    }

    Atom actual_type;
    int actual_format;
    unsigned long num_items;
    unsigned long bytes_after;
    unsigned char* property = NULL;
    bool pid_available = XGetWindowProperty(
        display.get(),
        parent,
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
    
    bool valid_results =
        pid_available && actual_type != None &&
        actual_format == 32 && num_items > 0;
    if(!valid_results) {
        if(property != NULL) {
            XFree(property);
        }
        return invalid_window;
    }

    long pid = ((long*)property)[0];
    
    XFree(property);

    unique_ptr<IActiveWindow> active_window(
        new LinuxActiveWindow(move(display), parent, pid)
    );
    return active_window;
}
