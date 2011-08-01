#ifndef GUARD_windows_com_setup_exception_h
#define GUARD_windows_com_setup_exception_h

#include <string>
#include <Windows.h>

#include "windows_exception.hpp"

class com_setup_exception : public windows_exception
{
    public:
        com_setup_exception(
            const string& m="An error occurred with setting up COM",
            HRESULT hr=0
        )
        {
            msg = m;
            errorcode = hr;
        }
};

#endif
