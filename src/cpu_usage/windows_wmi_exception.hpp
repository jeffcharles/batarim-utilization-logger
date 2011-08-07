#ifndef GUARD_windows_wmi_exception_h
#define GUARD_windows_wmi_exception_h

#include <string>
#include <Windows.h>

#include "windows_exception.hpp"

class wmi_exception : public windows_exception
{
    public:
        wmi_exception(
            const string& m="An error occurred with WMI", 
            HRESULT hr=0
        )
        { 
            msg = m;
            errorcode = hr;
        }
};

#endif
