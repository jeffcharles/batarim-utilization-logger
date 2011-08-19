#ifndef GUARD_active_window_h
#define GUARD_active_window_h

#include <string>

// Returns the name of the current active window
std::wstring get_active_window_name();

// Returns the name of the application that controls the  active window
std::wstring get_active_window_process_name();

#endif
