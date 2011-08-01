#ifndef GUARD_windows_com_setup_exception_h
#define GUARD_windows_com_setup_exception_h

#include <exception>

class com_setup_exception : public std::exception
{
    virtual const char* what() const throw()
    {
        return "An error occurred with setting up COM";
    }
};

#endif
