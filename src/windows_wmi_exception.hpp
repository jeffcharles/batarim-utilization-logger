#ifndef GUARD_windows_wmi_exception_h
#define GUARD_windows_wmi_exception_h

#include <exception>

class wmi_exception : public std::exception
{
    virtual const char* what() const throw()
    {
        return "An error occurred with WMI";
    }
};

#endif
